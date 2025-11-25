#include <CZ/Core/CZLog.h>
#include <CZ/Core/CZCore.h>
#include <CZ/Core/CZTimer.h>
#include <CZ/Core/CZAnimation.h>
#include <CZ/Core/CZLockGuard.h>
#include <CZ/Core/CZKeymap.h>
#include <CZ/Core/Events/CZEvent.h>
#include <sys/timerfd.h>

using namespace CZ;

static std::weak_ptr<CZCore> s_core;

std::shared_ptr<CZ::CZCore> CZCore::GetOrMake() noexcept
{
    auto core { s_core.lock() };

    if (core)
        return core;

    s_core = core = std::shared_ptr<CZCore>(new CZCore());

    if (!core->init())
        return {};

    return core;
}

std::shared_ptr<CZCore> CZCore::Get() noexcept
{
    return s_core.lock();
}

void CZCore::unlockLoop() noexcept
{
    m_loopUnlocker->setState(true);
}

int CZCore::dispatch(int msTimeout) noexcept
{
    updateEventSources();

    const auto ret { epoll_wait(m_epollFd, m_epollEvents.data(), m_epollEvents.size(), msTimeout) };

    if (ret == -1)
        return ret;

    CZEventSource *source;

    for (int i = 0; i < ret; i++)
    {
        source = static_cast<CZEventSource*>(m_epollEvents[i].data.ptr);

        if (!source->m_callback || source->m_self.use_count() == 1)
            continue;

        source->m_callback(source->fd(), m_epollEvents[i].events);
    }

    CZSafeEventQueue tmp { std::move(m_eventQueue) };
    tmp.dispatch();

    return ret;
}

bool CZCore::sendEvent(const CZEvent &event, CZObject &object) noexcept
{
    event.accept();

    if (event.type() == CZEvent::Type::Destroy)
    {
        delete &object;
        return true;
    }

    for (CZObject *filter : object.m_installedEventFilters)
        if (filter->eventFilter(event, object))
            return true;

    return object.event(event);
}

void CZCore::postEvent(std::shared_ptr<CZEvent> event, CZObject &object) noexcept
{
    if (!event) return;
    unlockLoop();
    m_eventQueue.addEvent(event, object);
}

CZCore::CZCore() noexcept
{
    CZLog(CZInfo, CZLN, "CZCore created");

    m_epollFd = epoll_create1(EPOLL_CLOEXEC);
}

CZCore::~CZCore() noexcept
{
    std::vector<CZTimer*> oneshotTimers;
    oneshotTimers.reserve(m_timers.size());

    for (size_t i = 0; i < m_timers.size();)
    {
        if (m_timers[i]->m_oneShoot)
        {
            oneshotTimers.emplace_back(m_timers[i]);
            m_timers[i] = m_timers.back();
            m_timers.pop_back();
            continue;
        }
        else
            i++;
    }

    while (!oneshotTimers.empty())
    {
        delete oneshotTimers.back();
        oneshotTimers.pop_back();
    }

    updateEventSources();

    while (!m_currentEventSources.empty())
    {
        epoll_ctl(m_epollFd, EPOLL_CTL_DEL, m_currentEventSources.back()->fd(), NULL);
        m_currentEventSources.pop_back();
    }

    close(m_epollFd);

    CZLog(CZInfo, CZLN, "CZCore destroyed");
}

bool CZCore::init() noexcept
{
    m_loopUnlocker = CZBooleanEventSource::Make(false, [this](auto) {
        CZSafeEventQueue tmp { std::move(m_eventQueue) };
        tmp.dispatch();
    });

    if (!initTimersSource())
        return false;

    if (!initKeymap())
        return false;

    m_animationsTimer = std::make_unique<CZTimer>();
    m_animationsTimer->setCallback([this](CZTimer*) {
        updateAnimations();
    });
    return true;
}

bool CZCore::initKeymap() noexcept
{
    auto keymap { CZKeymap::MakeServer({}) };

    if (!keymap)
    {
        unsetenv("XKB_DEFAULT_RULES");
        unsetenv("XKB_DEFAULT_MODEL");
        unsetenv("XKB_DEFAULT_LAYOUT");
        unsetenv("XKB_DEFAULT_VARIANT");
        unsetenv("XKB_DEFAULT_OPTIONS");
        keymap = CZKeymap::MakeServer({});
    }

    if (!keymap)
    {
        CZLog(CZFatal, CZLN, "Failed to create a default keymap");
        return false;
    }

    setKeymap(keymap);
    return true;
}

bool CZCore::initTimersSource() noexcept
{
    auto fd { timerfd_create(CLOCK_MONOTONIC, TFD_CLOEXEC) };

    if (fd < 0)
    {
        CZLog(CZFatal, CZLN, "Failed to create timerfd");
        return false;
    }

    m_timersSource = CZEventSource::Make(fd, EPOLLIN, CZOwn::Own, [this](int, UInt32) {
        updateTimers();
    });

    return true;
}

void CZCore::updateEventSources() noexcept
{
    // If an event source use_count == 1 => the user destroyed it
    for (size_t i = 0; i < m_currentEventSources.size();)
    {
        if (m_currentEventSources[i].use_count() == 1)
        {
            CZLog(CZDebug, CZLN, "Event source destroyed fd: {}", m_currentEventSources[i]->fd());
            epoll_ctl(m_epollFd, EPOLL_CTL_DEL, m_currentEventSources[i]->fd(), NULL);
            m_currentEventSources[i] = m_currentEventSources.back();
            m_currentEventSources.pop_back();
        }
        else
            i++;
    }

    // Add pending sources
    while (!m_pendingEventSources.empty())
    {
        m_currentEventSources.emplace_back(m_pendingEventSources.back());
        m_pendingEventSources.pop_back();
    }

    m_epollEvents.resize(m_currentEventSources.size());
}

void CZCore::updateTimers() noexcept
{
    UInt64 expirations;
    assert(read(m_timersSource->fd(), &expirations, sizeof(expirations)) == sizeof(expirations));

    // Disarm
    constexpr itimerspec timeout { {0, 0}, {0, 0} };
    timerfd_settime(m_timersSource->fd(), 0, &timeout, nullptr);

    for (auto *timer : m_timers)
        timer->m_processed = false;

    Int64 elapsed;

retry:
    m_timersChanged = false;

    for (CZTimer *t : m_timers)
    {
        if (t->m_processed)
            continue;

        t->m_processed = true;

        if (!t->m_running)
            continue;

        elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                      std::chrono::steady_clock::now() - t->m_beginTime).count();

        if (elapsed < t->timeoutMs())
            continue;

        CZWeak<CZTimer> ref { t };
        t->m_running = false;
        t->m_callback(t);

        if (!ref || m_timersChanged)
            goto retry;

        if (t->m_oneShoot && !t->running())
        {
            delete t;
            goto retry;
        }
    }

    scheduleTimer();
}

void CZCore::scheduleTimer() noexcept
{
    // Find the earliest timer to expire
    auto closest { std::chrono::steady_clock::time_point::max() };
    bool foundOne { false };

    for (CZTimer *t : m_timers)
    {
        if (!t->running())
            continue;

        foundOne = true;

        auto exp { t->m_beginTime + std::chrono::milliseconds(t->m_timeoutMs) };

        if (exp < closest)
            closest = exp;
    }

    if (!foundOne)
        return;

    Int64 ms { std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - closest).count() };

    if (ms <= 0)
        ms = 1;

    itimerspec timeout {};
    timeout.it_interval.tv_sec = 0;
    timeout.it_interval.tv_nsec = 0;
    timeout.it_value.tv_sec  = ms / 1000;
    timeout.it_value.tv_nsec = (ms % 1000) * 1'000'000;

    if (timerfd_settime(m_timersSource->fd(), 0, &timeout, nullptr) == -1)
    {
        CZLog(CZError, CZLN, "Failed to set timerfd");
        return;
    }
}

void CZCore::updateAnimations() noexcept
{
    bool anyRunning { false };

    for (CZAnimation *a : m_animations)
        a->m_processed = false;

retry:
    m_animationsChanged = false;

    for (CZAnimation *a : m_animations)
    {
        if (a->m_processed)
            continue;

        if (a->m_pendingDestroy)
        {
            delete a;
            goto retry;
        }

        a->m_processed = true;

        if (!a->isRunning())
            continue;

        a->onUpdate();

        if (a->isRunning())
        {
            anyRunning = true;

            if (a->m_onUpdate)
            {
                a->m_onUpdate(a);

                if (m_animationsChanged)
                    goto retry;
            }
        }
        else
        {
            a->stop();

            if (a->m_onFinish)
            {
                a->m_onFinish(a);

                if (m_animationsChanged)
                    goto retry;
            }
        }
    }

    if (anyRunning && m_animationInteval > 0)
        m_animationsTimer->start(m_animationInteval);
}

void CZCore::setAnimationInterval(UInt64 interval) noexcept
{
    if (interval == m_animationInteval)
        return;

    m_animationInteval = interval;

    if (m_animationInteval > 0)
        m_animationsTimer->start(m_animationInteval);
}

void CZCore::setKeymap(std::shared_ptr<CZKeymap> keymap) noexcept
{
    if (keymap == m_keymap)
        return;

    m_keymap = keymap;
    onKeymapChanged.notify();
}
