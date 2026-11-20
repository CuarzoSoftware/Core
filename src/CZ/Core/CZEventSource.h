#ifndef CZ_CZEVENTSOURCE_H
#define CZ_CZEVENTSOURCE_H

#include <CZ/Core/CZObject.h>
#include <CZ/Core/CZOwn.h>
#include <functional>
#include <memory>
#include <sys/epoll.h>

/**
 * @brief File descriptor monitored by the CZCore event loop.
 *
 * Registers a file descriptor in the CZCore epoll instance and invokes a callback from
 * CZCore::dispatch() whenever any of the requested events occur.
 *
 * The source stays registered while the user holds the `std::shared_ptr` returned by Make().
 * Once the last user reference is released, the callback is no longer invoked and the fd is
 * removed from the event loop (and closed if it was created with CZOwn::Own).
 */
class CZ::CZEventSource final : public CZObject
{
public:
    /** @brief Callback invoked with the fd, the triggered epoll events and the source itself. */
    using Callback = std::function<void(int fd, UInt32 events, CZEventSource *source)>;

    /**
     * @brief Creates an event source and adds it to the event loop.
     *
     * A CZCore must exist.
     *
     * @param fd The file descriptor to monitor.
     * @param events Epoll events to monitor (e.g. `EPOLLIN`), must not be 0.
     * @param own If CZOwn::Own, the fd is closed when the source is destroyed (or if creation fails).
     * @param callback Function invoked when events occur, must be valid.
     * @return The new event source, or `nullptr` on failure.
     */
    static std::shared_ptr<CZEventSource> Make(int fd, UInt32 events, CZOwn own, const Callback &callback) noexcept;

    /** @brief Destructor. Closes the fd if it is owned. */
    ~CZEventSource() noexcept;

    /** @brief The monitored file descriptor. */
    int fd() const noexcept { return m_fd; };

    /** @brief The monitored epoll events. */
    UInt32 events() const noexcept { return m_event.events; };

    /** @brief Changes the monitored epoll events. */
    void setEvents(UInt32 events) noexcept;
private:
    friend class CZCore;
    CZEventSource(int fd, UInt32 events, CZOwn own, const Callback &callback) noexcept : m_callback(callback), m_fd(fd), m_own(own)
    {
        m_event.events = events;
        m_event.data.ptr = this;
    }
    std::weak_ptr<CZEventSource> m_self;
    Callback m_callback;
    epoll_event m_event;
    int m_fd;
    CZOwn m_own;
};
#endif // CZ_CZEVENTSOURCE_H
