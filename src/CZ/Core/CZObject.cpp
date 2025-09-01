#include <CZ/Core/CZObject.h>
#include <CZ/Core/CZWeak.h>

using namespace CZ;

void CZObject::installEventFilter(CZObject *monitor) const noexcept
{
    if (!monitor)
        return;

    const auto &it { monitor->m_eventFilterSubscriptions.find((CZObject*)this) };

    // Already installed, move to front
    if (it != monitor->m_eventFilterSubscriptions.end())
        m_installedEventFilters.erase(it->second);

    m_installedEventFilters.push_front(monitor);
    monitor->m_eventFilterSubscriptions[(CZObject*)this] = m_installedEventFilters.begin();
}

void CZObject::removeEventFilter(CZObject *monitor) const noexcept
{
    if (!monitor)
        return;

    const auto &it { monitor->m_eventFilterSubscriptions.find((CZObject*)this) };

    if (it != monitor->m_eventFilterSubscriptions.end())
    {
        m_installedEventFilters.erase(it->second);
        monitor->m_eventFilterSubscriptions.erase(it);
    }
}

void CZObject::destroyLater() noexcept
{
    //TODO: akApp()->postEvent(AKDestroyEvent(), *this);
}

CZObject::~CZObject() noexcept
{
    notifyDestruction();

    while (!m_eventFilterSubscriptions.empty())
        m_eventFilterSubscriptions.begin()->first->removeEventFilter(this);

    while (!m_installedEventFilters.empty())
        removeEventFilter(m_installedEventFilters.back());
}

void CZObject::notifyDestruction() noexcept
{
    if (m_destroyed)
        return;

    m_destroyed = true;
    onDestroy.notify(this);

    while (!m_listeners.empty())
        delete m_listeners.back();

    while (!m_weakRefs.empty())
    {
        CZWeak<CZObject> *weak { (CZWeak<CZObject>*)m_weakRefs.back() };
        weak->m_object = nullptr;
        m_weakRefs.pop_back();

        if (weak->m_onDestroyCallback)
            (*weak->m_onDestroyCallback)(this);
    }
}
