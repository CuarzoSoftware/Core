#ifndef CZOBJECT_H
#define CZOBJECT_H

#include <CZ/CZSignal.h>
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

    /**
     * @brief Sets the user data.
     */
    void setUserData(void *data) const noexcept
    {
        m_userData = data;
    }

    /**
     * @brief Retrieves the stored user data.
     *
     * `nullptr` by default.
     */
    void *userData() const noexcept
    {
        return m_userData;
    }

    CZSignal<CZObject*> onDestroy;

    void installEventFilter(CZObject *monitor) const noexcept;
    void removeEventFilter(CZObject *monitor) const noexcept;
    void destroyLater() noexcept;
protected:

    /**
     * @brief Constructor of the CZObject class.
     */
    CZObject() noexcept = default;

    /**
     * @brief Destructor of the CZObject class.
     */
    virtual ~CZObject() noexcept;

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
    friend class CZApplication;
    mutable std::list<CZObject*> m_installedEventFilters;
    mutable std::unordered_map<CZObject*, std::list<CZObject*>::iterator> m_eventFilterSubscriptions;
    std::vector<CZListener*> m_listeners;
    mutable std::vector<void*> m_weakRefs;
    mutable void *m_userData { nullptr };
    bool m_destroyed { false };
};
#endif
