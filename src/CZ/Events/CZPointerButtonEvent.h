#ifndef CZ_CZPOINTERBUTTONEVENT_H
#define CZ_CZPOINTERBUTTONEVENT_H

#include <CZ/skia/core/SkPoint.h>
#include <CZ/Events/CZInputEvent.h>
#include <linux/input-event-codes.h>

class CZ::CZPointerButtonEvent : public CZInputEvent
{
public:
    CZ_EVENT_DECLARE_COPY
    CZPointerButtonEvent() noexcept : CZInputEvent(Type::PointerButton) {};

    /// @see #include <linux/input-event-codes.h>
    UInt32 button { BTN_LEFT };
    bool pressed {};
};

#endif // CZ_CZPOINTERBUTTONEVENT_H
