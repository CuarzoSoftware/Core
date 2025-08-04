#include <CZ/CZCore.h>

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
