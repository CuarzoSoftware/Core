#ifndef CZ_CZDESTROYEVENT_H
#define CZ_CZDESTROYEVENT_H

#include <CZ/Core/Events/CZEvent.h>

/**
 * @brief Destroys the target object.
 *
 * When delivered through CZCore::sendEvent() or CZCore::postEvent(), the target object is deleted
 * instead of receiving the event (it is never passed to event filters or CZObject::event()).
 * Posting it is a safe way to delete an object later, from the event loop.
 */
class CZ::CZDestroyEvent : public CZEvent
{
public:
    CZ_EVENT_DECLARE_COPY
    CZDestroyEvent() noexcept : CZEvent(Type::Destroy) {};
};

#endif // CZ_CZDESTROYEVENT_H
