#ifndef CZ_CZPOINTERPINCHBEGINEVENT_H
#define CZ_CZPOINTERPINCHBEGINEVENT_H

#include <CZ/Core/Events/CZInputEvent.h>
#include <CZ/skia/core/SkPoint.h>

/**
 * @brief A touchpad pinch gesture began.
 */
class CZ::CZPointerPinchBeginEvent : public CZInputEvent
{
public:
    CZ_EVENT_DECLARE_COPY
    CZPointerPinchBeginEvent() noexcept : CZInputEvent(Type::PointerPinchBegin) {};
    /** @brief Number of fingers involved in the gesture. */
    UInt32 fingers {};
};

#endif // CZ_CZPOINTERPINCHBEGINEVENT_H
