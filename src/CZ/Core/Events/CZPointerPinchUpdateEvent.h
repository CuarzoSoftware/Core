#ifndef CZ_CZPOINTERPINCHUPDATEEVENT_H
#define CZ_CZPOINTERPINCHUPDATEEVENT_H

#include <CZ/Core/Events/CZInputEvent.h>
#include <CZ/skia/core/SkPoint.h>

/**
 * @brief A touchpad pinch gesture was updated.
 */
class CZ::CZPointerPinchUpdateEvent : public CZInputEvent
{
public:
    CZ_EVENT_DECLARE_COPY
    CZPointerPinchUpdateEvent() noexcept : CZInputEvent(Type::PointerPinchUpdate) {};
    /** @brief Number of fingers involved in the gesture. */
    UInt32 fingers {};
    /** @brief Motion of the gesture's center since the previous event, with pointer acceleration applied. */
    SkPoint delta {};
    /** @brief Motion of the gesture's center since the previous event, without pointer acceleration. */
    SkPoint deltaUnaccelerated {};
    /** @brief Absolute scale: current distance between the fingers divided by the distance when the gesture began. */
    Float32 scale {};
    /** @brief Rotation since the previous event, in degrees (clockwise is positive). */
    Float32 rotation {};
};

#endif // CZ_CZPOINTERPINCHUPDATEEVENT_H
