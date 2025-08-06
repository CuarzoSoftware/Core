#ifndef CZ_CZKEYBOARDENTEREVENT_H
#define CZ_CZKEYBOARDENTEREVENT_H

#include <CZ/Events/CZInputEvent.h>

class CZ::CZKeyboardEnterEvent : public CZInputEvent
{
public:
    CZ_EVENT_DECLARE_COPY
    CZKeyboardEnterEvent() noexcept : CZInputEvent(Type::KeyboardEnter) {};
};

#endif // CZ_CZKEYBOARDENTEREVENT_H
