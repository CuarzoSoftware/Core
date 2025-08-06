#ifndef CZ_CZTOUCHFRAMEEVENT_H
#define CZ_CZTOUCHFRAMEEVENT_H

#include <CZ/Events/CZInputEvent.h>

class CZ::CZTouchFrameEvent : public CZInputEvent
{
public:
    CZ_EVENT_DECLARE_COPY
    CZTouchFrameEvent() noexcept : CZInputEvent(Type::TouchFrame) {};
};

#endif // CZ_CZTOUCHFRAMEEVENT_H
