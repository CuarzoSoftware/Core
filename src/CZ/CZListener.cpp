#include <CZ/CZSignal.h>
#include <CZ/CZObject.h>

using namespace CZ;

CZListener::~CZListener()
{
    signal->listeners[signalLink] = signal->listeners.back();
    signal->listeners[signalLink]->signalLink = signalLink;
    signal->listeners.pop_back();

    object->m_listeners[objectLink] = object->m_listeners.back();
    object->m_listeners[objectLink]->objectLink = objectLink;
    object->m_listeners.pop_back();
}

CZListener::CZListener(CZObject *object, CZSignalBase *signal) noexcept :
    signal(signal),
    object(object)
{
    object->m_listeners.push_back(this);
    objectLink = object->m_listeners.size() - 1;
    signalLink = signal->listeners.size();
}
