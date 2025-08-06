#ifndef CZ_CZPOINTERPINCHBEGINEVENT_H
#define CZ_CZPOINTERPINCHBEGINEVENT_H

#include <CZ/Events/CZInputEvent.h>
#include <CZ/skia/core/SkPoint.h>

class CZ::CZPointerPinchBeginEvent : public CZInputEvent
{
public:
    CZ_EVENT_DECLARE_COPY
    CZPointerPinchBeginEvent() noexcept : CZInputEvent(Type::PointerPinchBegin) {};
    UInt32 fingers {};
};

#endif // CZ_CZPOINTERPINCHBEGINEVENT_H
