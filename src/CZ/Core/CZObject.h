#ifndef CZOBJECT_H
#define CZOBJECT_H

#include <CZ/Core/CZSignal.h>
#include <unordered_map>
#include <list>

/**
 * @brief Polymorphic root of CZObject (only provides a virtual destructor).
 */
class CZ::CZObjectBase
{
public:
    CZObjectBase() noexcept = default;
    virtual ~CZObjectBase() noexcept = default;
};

/**
 * @brief Base class for objects.
 *
 * Provides the features shared by all Cuarzo objects:
 * - Signal subscriptions: an object can own CZSignal subscriptions (see CZSignal::subscribe()),
 *   which are removed automatically when it is destroyed.
 * - Weak references: CZWeak references to the object are set to `nullptr` when it is destroyed.
 * - Events: events sent to the object through CZCore::sendEvent() are first passed to its installed
 *   event filters (see installEventFilter()) and then to event().
 *
 * Objects can't be copied or moved.
 *
 * @see CZWeak
 */
class CZ::CZObject : public CZObjectBase
{
public:
    CZ_DISABLE_COPY(CZObject)

    /** @brief Arbitrary user data pointer, not used by Cuarzo. */
    void *userData {};

    /** @brief Emitted once when the object is being destroyed (see notifyDestruction()). */
    CZSignal<CZObject*> onDestroy;

    /**
     * @brief Installs @p monitor as an event filter of this object.
     *
     * Events sent to this object through CZCore::sendEvent() are first passed to the eventFilter()
     * of its monitors, most recently installed first. If a filter returns `true`, the event is not
     * delivered further. Installing an already installed monitor moves it to the front.
     * The filter is removed automatically when either object is destroyed.
     */
    void installEventFilter(CZObject *monitor) const noexcept;

    /** @brief Removes an event filter previously installed with installEventFilter(). */
    void removeEventFilter(CZObject *monitor) const noexcept;

    /** @brief Intended to schedule the object's destruction. Currently not implemented (does nothing). */
    void destroyLater() noexcept;

    /**
     * @brief Constructor of the CZObject class.
     */
    CZObject() noexcept = default;

    /**
     * @brief Destructor of the CZObject class.
     */
    ~CZObject() noexcept;

protected:

    /**
     * @brief Notifies the object destruction.
     *
     * This method can be invoked from a subclass destructor to notify the object's imminent destruction
     * to all associated CZWeak references in advance. If not invoked, the base CZObject automatically calls it.
     *
     * After invocation, all CZWeak references are set to `nullptr`, preventing the creation of additional references for this object.
     */
    void notifyDestruction() noexcept;

    /**
     * @brief Handles an event sent to this object.
     *
     * Called by CZCore::sendEvent() if no event filter handled it. The default implementation does nothing.
     *
     * @return `true` if the event was handled, `false` otherwise.
     */
    virtual bool event(const CZEvent &event) noexcept
    {
        CZ_UNUSED(event)
        return false;
    }

    /**
     * @brief Intercepts events sent to objects this object monitors (see installEventFilter()).
     *
     * The default implementation does nothing.
     *
     * @param event The event.
     * @param target The object the event was sent to.
     * @return `true` to stop the event from being delivered to further filters and to @p target.
     */
    virtual bool eventFilter(const CZEvent &event, CZObject &target) noexcept
    {
        CZ_UNUSED(event)
        CZ_UNUSED(target)
        return false;
    }

private:
    friend class CZWeakUtils;
    friend class CZListener;
    friend class CZCore;
    mutable std::list<CZObject*> m_installedEventFilters;
    mutable std::unordered_map<CZObject*, std::list<CZObject*>::iterator> m_eventFilterSubscriptions;
    std::vector<CZListener*> m_listeners;
    mutable std::vector<void*> m_weakRefs;
    bool m_destroyed { false };
};
#endif
