#ifndef CZ_CZKEYBOARDLEAVEEVENT_H
#define CZ_CZKEYBOARDLEAVEEVENT_H

#include <CZ/Events/CZInputEvent.h>

class CZ::CZKeyboardLeaveEvent : public CZInputEvent
{
public:
    CZ_EVENT_DECLARE_COPY
    CZKeyboardLeaveEvent() noexcept : CZInputEvent(Type::KeyboardLeave) {};
};

#endif // CZ_CZKEYBOARDLEAVEEVENT_H
