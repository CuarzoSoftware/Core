#ifndef CZ_CZPOINTERBUTTONEVENT_H
#define CZ_CZPOINTERBUTTONEVENT_H

#include <CZ/skia/core/SkPoint.h>
#include <CZ/Core/Events/CZInputEvent.h>
#include <linux/input-event-codes.h>

/**
 * @brief A pointer button was pressed or released.
 */
class CZ::CZPointerButtonEvent : public CZInputEvent
{
public:
    CZ_EVENT_DECLARE_COPY
    CZPointerButtonEvent() noexcept : CZInputEvent(Type::PointerButton) {};

    /** @brief The button code (e.g. `BTN_LEFT`), see `<linux/input-event-codes.h>`. */
    UInt32 button { BTN_LEFT };
    /** @brief `true` if the button was pressed, `false` if released. */
    bool pressed {};
};

#endif // CZ_CZPOINTERBUTTONEVENT_H
