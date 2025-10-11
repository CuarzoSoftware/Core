#ifndef CZ_CZKEYBOARDKEYEVENT_H
#define CZ_CZKEYBOARDKEYEVENT_H

#include <CZ/Core/Events/CZInputEvent.h>
#include <xkbcommon/xkbcommon-compose.h>

class CZ::CZKeyboardKeyEvent : public CZInputEvent
{
public:
    CZ_EVENT_DECLARE_COPY
    CZKeyboardKeyEvent() noexcept : CZInputEvent(Type::KeyboardKey) {};

    mutable bool isPressed {};
    mutable bool isRepeat {};

    /// @see #include <linux/input-event-codes.h>
    mutable UInt32 code {};
    mutable xkb_keysym_t symbol {};

    mutable xkb_compose_status composeStatus { XKB_COMPOSE_NOTHING };
    mutable std::string utf8 {};
};

#endif // CZ_CZKEYBOARDKEYEVENT_H
