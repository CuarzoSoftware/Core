#ifndef CZ_CZSAFEEVENTQUEUE_H
#define CZ_CZSAFEEVENTQUEUE_H

#include <CZ/Core/CZLockGuard.h>
#include <CZ/Core/CZObject.h>
#include <CZ/Core/CZWeak.h>
#include <CZ/Core/Events/CZEvent.h>
#include <memory>
#include <queue>

/**
 * @brief Event queue with weak object references.
 *
 * This queue stores weak references to target objects. Before dispatching events,
 * it checks if the objects are still alive and discards them otherwise.
 *
 * Use this in situations where objects may be destroyed while handling events in the queue.
 *
 * Internally, it triggers CZCore::sendEvent().
 */
class CZ::CZSafeEventQueue
{
public:

    /**
     * @brief Default constructor.
     */
    CZSafeEventQueue() noexcept = default;

    /**
     * @brief Move constructor.
     *
     * After the move, the other queue is left empty and can be safely reused.
     */
    CZSafeEventQueue(CZSafeEventQueue &&other) noexcept
    {
        *this = std::move(other);
    }

    /**
     * @brief Move assignment operator.
     *
     * Transfers the contents from another AKSafeEventQueue to this one.
     *
     * After the move, the other queue is left empty and can be safely reused.
     *
     * @param other The other AKSafeEventQueue to move from.
     * @return A reference to this AKSafeEventQueue.
     */
    CZSafeEventQueue &operator=(CZSafeEventQueue &&other) noexcept
    {
        if (this != &other)
        {
            m_queue = std::move(other.m_queue);
            other.m_queue = std::queue<SafeEvent>();
        }
        return *this;
    }

    CZSafeEventQueue(const CZSafeEventQueue&) = delete;
    CZSafeEventQueue& operator=(const CZSafeEventQueue&) = delete;

    /**
     * @brief Adds an event to the queue.
     *
     * @param event The event to be added.
     * @param object The target object associated with the event.
     */
    void addEvent(std::shared_ptr<CZEvent> event, CZObject &object) noexcept;

    /**
     * @brief Dispatches events in the queue.
     *
     * This method processes each event in the queue, discarding events
     * associated with destroyed objects and triggering AKApplication::sendEvent()
     * for remaining events.
     */
    void dispatch() noexcept;

private:

    struct SafeEvent
    {
        CZWeak<CZObject> object;
        std::shared_ptr<CZEvent> event;
    };

    std::queue<SafeEvent> m_queue;
};

#endif // CZ_CZSAFEEVENTQUEUE_H
