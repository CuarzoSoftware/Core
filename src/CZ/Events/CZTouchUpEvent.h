#ifndef CZ_CZTOUCHUPEVENT_H
#define CZ_CZTOUCHUPEVENT_H

#include <CZ/Events/CZInputEvent.h>
#include <CZ/skia/core/SkPoint.h>

class CZ::CZTouchUpEvent : public CZInputEvent
{
public:
    CZ_EVENT_DECLARE_COPY
    CZTouchUpEvent() noexcept : CZInputEvent(Type::TouchUp) {};
    Int32 id {};
};

#endif // CZ_CZTOUCHUPEVENT_H
