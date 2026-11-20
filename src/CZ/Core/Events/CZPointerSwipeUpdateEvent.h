#ifndef CZ_CZPOINTERSWIPEUPDATE_H
#define CZ_CZPOINTERSWIPEUPDATE_H

#include <CZ/Core/Events/CZInputEvent.h>
#include <CZ/skia/core/SkPoint.h>

/**
 * @brief A touchpad swipe gesture was updated.
 */
class CZ::CZPointerSwipeUpdateEvent : public CZInputEvent
{
public:
    CZ_EVENT_DECLARE_COPY
    CZPointerSwipeUpdateEvent() noexcept : CZInputEvent(Type::PointerSwipeUpdate) {};
    /** @brief Number of fingers involved in the gesture. */
    UInt32 fingers {};
    /** @brief Motion of the gesture's center since the previous event, with pointer acceleration applied. */
    SkPoint delta {};
    /** @brief Motion of the gesture's center since the previous event, without pointer acceleration. */
    SkPoint deltaUnaccelerated {};
};

#endif // CZ_CZPOINTERSWIPEUPDATE_H
