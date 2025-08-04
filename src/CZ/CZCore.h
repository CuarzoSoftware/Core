#ifndef CZCORE_H
#define CZCORE_H

#include <CZ/Cuarzo.h>
#include <CZ/CZEventSource.h>
#include <CZ/CZSafeEventQueue.h>
#include <CZ/CZBooleanEventSource.h>
#include <memory>
#include <vector>
#include <sys/epoll.h>

class CZ::CZCore
{
public:
    static std::shared_ptr<CZCore> Get() noexcept;
    int fd() const noexcept { return m_epollFd; }
    void unlockLoop() noexcept;
    int dispatch(int msTimeout = -1) noexcept;
private:
    friend class CZEventSource;
    CZCore() noexcept;
    ~CZCore() noexcept;
    void init() noexcept;
    void updateEventSources() noexcept;
    int m_epollFd;
    std::vector<epoll_event> m_epollEvents;
    std::vector<std::shared_ptr<CZEventSource>> m_currentEventSources;
    std::vector<std::shared_ptr<CZEventSource>> m_pendingEventSources;
    std::shared_ptr<CZBooleanEventSource> m_loopUnlocker;
    CZSafeEventQueue m_eventQueue;
};

#endif // CZCORE_H
