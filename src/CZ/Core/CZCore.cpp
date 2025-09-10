#include <CZ/Core/CZCore.h>
#include <CZ/Core/CZTimer.h>
#include <CZ/Core/CZAnimation.h>
#include <CZ/Core/CZLockGuard.h>
#include <CZ/Events/CZEvent.h>

using namespace CZ;

static std::weak_ptr<CZCore> s_core;

std::shared_ptr<CZCore> CZCore::Get() noexcept
{
    auto core { s_core.lock() };

    if (core)
        return core;

    s_core = core = std::shared_ptr<CZCore>(new CZCore());
    core->init();
    return core;
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

void CZCore::postEvent(const CZEvent &event, CZObject &object) noexcept
{
    unlockLoop();
    m_eventQueue.addEvent(event, object);
}

CZCore::CZCore() noexcept
{
    m_epollFd = epoll_create1(EPOLL_CLOEXEC);
}

CZCore::~CZCore() noexcept
{
    updateEventSources();

    while (!m_currentEventSources.empty())
    {
        epoll_ctl(m_epollFd, EPOLL_CTL_DEL, m_currentEventSources.back()->fd(), NULL);
        m_currentEventSources.pop_back();
    }

    close(m_epollFd);
}

void CZCore::init() noexcept
{
    m_loopUnlocker = CZBooleanEventSource::Make(false, [this](auto) {
        CZSafeEventQueue tmp { std::move(m_eventQueue) };
        tmp.dispatch();
    });

    m_animationsTimer = std::make_unique<CZTimer>();
    m_animationsTimer->setCallback([this](CZTimer*) {
        updateAnimations();
    });
}

void CZCore::updateEventSources() noexcept
{
    // If an event source use_count == 1 => the user destroyed it
    for (size_t i = 0; i < m_currentEventSources.size();)
    {
        if (m_currentEventSources[i].use_count() == 1)
        {
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

void CZCore::updateAnimations() noexcept
{
    Int64 elapsed;
    Int64 duration;
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

        if (!a->m_running)
            continue;

        elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                      std::chrono::steady_clock::now() - a->m_beginTime).count();

        duration = static_cast<Int64>(a->m_duration);

        if (elapsed >= duration)
            a->m_value = 1.0;
        else
            a->m_value = static_cast<Float64>(elapsed)/static_cast<Float64>(duration);

        if (a->m_onUpdate)
        {
            anyRunning = true;
            a->m_onUpdate(a);

            if (m_animationsChanged)
                goto retry;
        }

        if (a->m_value == 1.0)
        {
            a->stop();

            if (m_animationsChanged)
                goto retry;
        }
    }

    if (anyRunning && m_autoUpdateAnimations)
        m_animationsTimer->start(8);
}

void CZCore::setAutoUpdateAnimations(bool autoUpdate) noexcept
{
    if (autoUpdate == m_autoUpdateAnimations)
        return;

    m_autoUpdateAnimations = autoUpdate;

    if (autoUpdate)
        m_animationsTimer->start(8);
}
