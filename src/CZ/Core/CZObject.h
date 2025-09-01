#ifndef CZOBJECT_H
#define CZOBJECT_H

#include <CZ/Core/CZSignal.h>
#include <unordered_map>
#include <list>

/**
 * @brief Base class for objects.
 *
 * @see CZWeak
 */
class CZ::CZObject
{
public:
    CZ_DISABLE_COPY(CZObject)

    void *userData {};
    CZSignal<CZObject*> onDestroy;

    void installEventFilter(CZObject *monitor) const noexcept;
    void removeEventFilter(CZObject *monitor) const noexcept;
    void destroyLater() noexcept;

    /**
     * @brief Constructor of the CZObject class.
     */
    CZObject() noexcept = default;

    /**
     * @brief Destructor of the CZObject class.
     */
    virtual ~CZObject() noexcept;

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

    virtual bool event(const CZEvent &event) noexcept
    {
        CZ_UNUSED(event)
        return false;
    }

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
