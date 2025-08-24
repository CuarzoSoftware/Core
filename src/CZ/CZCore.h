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

    /// Sent immediately, true if accepted, false if ignored
    bool sendEvent(const CZEvent &event, CZObject &object) noexcept;

    /// Enqueued and sent later
    void postEvent(const CZEvent &event, CZObject &object) noexcept;

    void updateAnimations() noexcept;
    bool autoUpdateAnimations() const noexcept { return m_autoUpdateAnimations; };
    void setAutoUpdateAnimations(bool autoUpdate) noexcept;

    ~CZCore() noexcept;
private:
    friend class CZEventSource;
    friend class CZAnimation;
    friend class LCompositor;

    enum class Owner
    {
        None,
        Louvre,
        Marco
    };

    CZCore() noexcept;
    void init() noexcept;
    void updateEventSources() noexcept;
    int m_epollFd;
    std::vector<epoll_event> m_epollEvents;
    std::vector<std::shared_ptr<CZEventSource>> m_currentEventSources;
    std::vector<std::shared_ptr<CZEventSource>> m_pendingEventSources;
    std::shared_ptr<CZBooleanEventSource> m_loopUnlocker;
    CZSafeEventQueue m_eventQueue;
    Owner m_owner { Owner::None };

    std::vector<CZAnimation*> m_animations;
    std::unique_ptr<CZTimer> m_animationsTimer;
    bool m_animationsChanged { false };
    bool m_autoUpdateAnimations { true };
};

#endif // CZCORE_H
