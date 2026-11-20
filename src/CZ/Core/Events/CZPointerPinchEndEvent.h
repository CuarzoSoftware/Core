#ifndef CZ_CZPOINTERPINCHENDEVENT_H
#define CZ_CZPOINTERPINCHENDEVENT_H

#include <CZ/Core/Events/CZInputEvent.h>
#include <CZ/skia/core/SkPoint.h>

/**
 * @brief A touchpad pinch gesture ended.
 */
class CZ::CZPointerPinchEndEvent : public CZInputEvent
{
public:
    CZ_EVENT_DECLARE_COPY
    CZPointerPinchEndEvent() noexcept : CZInputEvent(Type::PointerPinchEnd) {};
    /** @brief Number of fingers involved in the gesture. */
    UInt32 fingers {};
    /** @brief `true` if the gesture was cancelled instead of completed. */
    bool cancelled {};
};

#endif // CZ_CZPOINTERPINCHENDEVENT_H
