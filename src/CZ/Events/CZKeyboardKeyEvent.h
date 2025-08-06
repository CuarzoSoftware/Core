#ifndef CZ_CZKEYBOARDKEYEVENT_H
#define CZ_CZKEYBOARDKEYEVENT_H

#include <CZ/Events/CZInputEvent.h>

class CZ::CZKeyboardKeyEvent : public CZInputEvent
{
public:
    CZ_EVENT_DECLARE_COPY
    CZKeyboardKeyEvent() noexcept : CZInputEvent(Type::KeyboardKey) {};

    /// @see #include <linux/input-event-codes.h>
    UInt32 keyCode {};
    bool pressed {};
};

#endif // CZ_CZKEYBOARDKEYEVENT_H
