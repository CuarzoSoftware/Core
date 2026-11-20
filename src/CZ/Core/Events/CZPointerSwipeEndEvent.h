#ifndef CZ_CZPOINTERSWIPEENDEVENT_H
#define CZ_CZPOINTERSWIPEENDEVENT_H

#include <CZ/Core/Events/CZInputEvent.h>
#include <CZ/skia/core/SkPoint.h>

/**
 * @brief A touchpad swipe gesture ended.
 */
class CZ::CZPointerSwipeEndEvent : public CZInputEvent
{
public:
    CZ_EVENT_DECLARE_COPY
    CZPointerSwipeEndEvent() noexcept : CZInputEvent(Type::PointerSwipeEnd) {};
    /** @brief Number of fingers involved in the gesture. */
    UInt32 fingers {};
    /** @brief `true` if the gesture was cancelled instead of completed. */
    bool cancelled {};
};

#endif // CZ_CZPOINTERSWIPEENDEVENT_H
