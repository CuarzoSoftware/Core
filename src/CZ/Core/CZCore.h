#ifndef CZCORE_H
#define CZCORE_H

#include <CZ/Core/Cuarzo.h>
#include <CZ/Core/CZEventSource.h>
#include <CZ/Core/CZSafeEventQueue.h>
#include <CZ/Core/CZBooleanEventSource.h>
#include <memory>
#include <vector>
#include <sys/epoll.h>

/**
 * @brief Event loop manager.
 *
 * CZCore manages the main event loop and serves as a centralized hub
 * for registering event sources.
 *
 * Multiple projects within the Cuarzo framework such as SRM, Kay, Louvre, and Marco
 * automatically register their event sources into the same shared core instance.
 *
 * External sources are also supported and can be integrated with CZEventSource.
 *
 * It also manages object events with synchronous (send) and asynchronous (post) dispatch,
 * event filtering, timers, animations, and global keymaps.
 */
class CZ::CZCore
{
public:
    /**
     * @brief Retrieves the singleton instance of CZCore.
     *
     * If an instance does not already exist, it will be created.
     * This method ensures centralized access to the core event loop manager.
     *
     * @return Shared pointer to the CZCore instance.
     */
    static std::shared_ptr<CZCore> GetOrMake() noexcept;

    /**
     * @brief Retrieves the singleton instance of CZCore.
     *
     * @return Shared pointer to the CZCore instance or nullptr if not created.
     */
    static std::shared_ptr<CZCore> Get() noexcept;

    /**
     * @brief Returns the pollable file descriptor of the main event loop.
     *
     * This file descriptor can be used with external polling mechanisms
     * to integrate CZCore's event loop with other systems.
     *
     * @return File descriptor associated with the main loop.
     */
    int fd() const noexcept { return m_epollFd; }

    /**
     * @brief Unblocks the main event loop if it is currently waiting.
     *
     * Useful for waking the loop from external threads or interrupting
     * blocking operations.
     */
    void unlockLoop() noexcept;

    /**
     * @brief Runs the main event loop and dispatches events.
     *
     * Processes pending events, timers, animations, and input mappings.
     *
     * @param msTimeout Timeout in milliseconds. A value of -1 blocks indefinitely.
     * @return Number of dispatched events or -1 on error.
     */
    int dispatch(int msTimeout = -1) noexcept;

    /**
     * @brief Sends an event synchronously to the specified object.
     *
     * The event is delivered immediately. Returns true if the event was accepted,
     * false if it was ignored.
     *
     * @param event The event to send.
     * @param object The target object.
     * @return True if the event was accepted, false otherwise.
     */
    bool sendEvent(const CZEvent &event, CZObject &object) noexcept;

    /**
     * @brief Posts an event asynchronously to the specified object.
     *
     * The event is queued and dispatched during the next loop iteration.
     *
     * @param event Shared pointer to the event to post.
     * @param object The target object.
     */
    void postEvent(std::shared_ptr<CZEvent> event, CZObject &object) noexcept;

    /**
     * @brief Updates all running animations.
     *
     * Can be called manually for fine-grained control, such as syncing with
     * a screen refresh rate.
     */
    void updateAnimations() noexcept;

    /**
     * @brief Gets the interval in milliseconds for automatic animation updates.
     *
     * Defaults to 8 ms. Setting the interval to 0 disables automatic updates,
     * requiring manual calls to updateAnimations().
     *
     * @return Current animation update interval in milliseconds.
     */
    UInt64 animationInterval() const noexcept { return m_animationInteval; }

    /**
     * @brief Sets the interval in milliseconds for automatic animation updates.
     *
     * @param interval Update interval in milliseconds. Set to 0 to disable automatic updates.
     */
    void setAnimationInterval(UInt64 interval) noexcept;

    /**
     * @brief Returns the number of currently active animations.
     *
     * @return Count of running animations.
     */
    size_t animationCount() const noexcept { return m_animations.size(); }

    void setKeymap(std::shared_ptr<CZKeymap> keymap) noexcept;
    std::shared_ptr<CZKeymap> keymap() const noexcept { return m_keymap; }
    CZSignal<> onKeymapChanged;

    ~CZCore() noexcept;
private:
    friend class CZEventSource;
    friend class CZAnimation;
    friend class CZTimer;
    friend class LCompositor;
    friend class LKeyboard;

    enum class Owner
    {
        None,
        Louvre,
        Marco
    };

    CZCore() noexcept;
    bool init() noexcept;
    bool initKeymap() noexcept;
    bool initTimersSource() noexcept;
    void updateEventSources() noexcept;
    void updateTimers() noexcept;
    void scheduleTimer() noexcept;
    int m_epollFd;
    std::vector<epoll_event> m_epollEvents;
    std::vector<std::shared_ptr<CZEventSource>> m_currentEventSources;
    std::vector<std::shared_ptr<CZEventSource>> m_pendingEventSources;
    std::shared_ptr<CZBooleanEventSource> m_loopUnlocker;
    CZSafeEventQueue m_eventQueue;
    Owner m_owner { Owner::None };

    std::shared_ptr<CZEventSource> m_timersSource;
    std::vector<CZTimer*> m_timers;
    bool m_timersChanged { false };

    std::vector<CZAnimation*> m_animations;
    std::unique_ptr<CZTimer> m_animationsTimer;
    bool m_animationsChanged { false };
    UInt64 m_animationInteval { 8 };

    std::shared_ptr<CZKeymap> m_keymap;
};

#endif // CZCORE_H
