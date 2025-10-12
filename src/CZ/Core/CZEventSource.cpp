#include <CZ/Core/CZEventSource.h>
#include <CZ/Core/CZCore.h>
#include <CZ/Core/CZLog.h>
#include <sys/poll.h>

using namespace CZ;

std::shared_ptr<CZEventSource> CZEventSource::Make(int fd, UInt32 events, CZOwn own, const Callback &callback) noexcept
{
    auto core { CZCore::Get() };

    if (core.use_count() == 1)
    {
        CZLog(CZError, CZLN, "Missing CZCore");
        if (own == CZOwn::Own) close(fd);
        return {};
    }

    if (fd < 0)
    {
        CZLog(CZError, CZLN, "Invalid fd {}", fd);
        if (own == CZOwn::Own) close(fd);
        return {};
    }

    if (events == 0)
    {
        CZLog(CZError, CZLN, "Invalid events {}", events);
        if (own == CZOwn::Own) close(fd);
        return {};
    }

    if (!callback)
    {
        CZLog(CZError, CZLN, "Invalid callback");
        if (own == CZOwn::Own) close(fd);
        return {};
    }

    auto eventSource { std::shared_ptr<CZEventSource>(new CZEventSource(fd, events, own, callback)) };
    eventSource->m_self = eventSource;

    if (epoll_ctl(core->m_epollFd, EPOLL_CTL_ADD, eventSource->fd(), &eventSource->m_event) == -1)
    {
        CZLog(CZError, CZLN, "EPOLL_CTL_ADD failed");
        if (own == CZOwn::Own) close(fd);
        return {};
    }

    CZLog(CZDebug, CZLN, "Event source added fd: {}", fd);
    core->m_pendingEventSources.emplace_back(eventSource);
    return eventSource;
}

CZEventSource::~CZEventSource() noexcept
{
    if (m_own == CZOwn::Own)
        close(m_fd);
}
