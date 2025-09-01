#ifndef CZ_CZEVENTSOURCE_H
#define CZ_CZEVENTSOURCE_H

#include <CZ/Core/CZObject.h>
#include <functional>
#include <memory>
#include <sys/epoll.h>

class CZ::CZEventSource final : public CZObject
{
public:
    using Callback = std::function<void(int fd, UInt32 events)>;

    static std::shared_ptr<CZEventSource> Make(int fd, UInt32 events, const Callback &callback) noexcept;
    int fd() const noexcept { return m_fd; };
    UInt32 events() const noexcept { return m_event.events; };
private:
    friend class CZCore;
    CZEventSource(int fd, UInt32 events, const Callback &callback) noexcept : m_callback(callback), m_fd(fd)
    {
        m_event.events = events;
        m_event.data.ptr = this;
    }
    std::weak_ptr<CZEventSource> m_self;
    Callback m_callback;
    epoll_event m_event;
    int m_fd;
};
#endif // CZ_CZEVENTSOURCE_H
