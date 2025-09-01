#include <CZ/Core/CZEventSource.h>
#include <CZ/Core/CZCore.h>
#include <CZ/Core/CZLog.h>
#include <sys/poll.h>

using namespace CZ;

std::shared_ptr<CZEventSource> CZEventSource::Make(int fd, UInt32 events, const Callback &callback) noexcept
{
    auto core { CZCore::Get() };

    if (core.use_count() == 1)
    {
        CZLog(CZError, CZLN, "Missing CZCore");
        return {};
    }

    if (fd < 0)
    {
        CZLog(CZError, CZLN, "Invalid fd {}", fd);
        return {};
    }

    if (events == 0)
    {
        CZLog(CZError, CZLN, "Invalid events {}", events);
        return {};
    }

    if (!callback)
    {
        CZLog(CZError, CZLN, "Invalid callback");
        return {};
    }

    auto eventSource { std::shared_ptr<CZEventSource>(new CZEventSource(fd, events, callback)) };
    eventSource->m_self = eventSource;

    if (epoll_ctl(core->m_epollFd, EPOLL_CTL_ADD, eventSource->fd(), &eventSource->m_event) == -1)
    {
        CZLog(CZError, CZLN, "EPOLL_CTL_ADD failed");
        return {};
    }

    core->m_pendingEventSources.emplace_back(eventSource);
    return eventSource;
}
