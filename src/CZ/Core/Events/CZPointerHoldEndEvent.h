#ifndef CZ_CZPOINTERHOLDENDEVENT_H
#define CZ_CZPOINTERHOLDENDEVENT_H

#include <CZ/Core/Events/CZInputEvent.h>
#include <CZ/skia/core/SkPoint.h>

/**
 * @brief A touchpad hold gesture ended.
 */
class CZ::CZPointerHoldEndEvent : public CZInputEvent
{
public:
    CZ_EVENT_DECLARE_COPY
    CZPointerHoldEndEvent() noexcept : CZInputEvent(Type::PointerHoldEnd) {};
    /** @brief Number of fingers involved in the gesture. */
    UInt32 fingers {};
    /** @brief `true` if the gesture was cancelled instead of completed. */
    bool cancelled {};
};

#endif // CZ_CZPOINTERHOLDENDEVENT_H
