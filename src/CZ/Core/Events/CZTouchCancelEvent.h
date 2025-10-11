#ifndef CZ_CZTOUCHCANCELEVENT_H
#define CZ_CZTOUCHCANCELEVENT_H

#include <CZ/Core/Events/CZInputEvent.h>

class CZ::CZTouchCancelEvent : public CZInputEvent
{
public:
    CZ_EVENT_DECLARE_COPY
    CZTouchCancelEvent() noexcept : CZInputEvent(Type::TouchCancel) {};
};

#endif // CZ_CZTOUCHCANCELEVENT_H
