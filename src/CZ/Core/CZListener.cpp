#include <CZ/Core/CZSignal.h>
#include <CZ/Core/CZObject.h>

using namespace CZ;

CZListener::~CZListener()
{
    // A listener may be destroyed while its signal is being emitted (e.g. its owner is deleted
    // from another listener's callback). The removal below swaps/pops the signal's listener vector,
    // so tell notify() to restart its iteration instead of reading an invalidated range.
    signal->changed = true;

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

    // Subscribing during an emission may reallocate the listener vector: make notify() restart its
    // iteration. The new listener is marked as notified so it doesn't receive the ongoing emission
    // (notify() resets every flag when it starts).
    signal->changed = true;
    notified = true;
}
