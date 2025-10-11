#ifndef CZ_CZPOINTERPINCHENDEVENT_H
#define CZ_CZPOINTERPINCHENDEVENT_H

#include <CZ/Core/Events/CZInputEvent.h>
#include <CZ/skia/core/SkPoint.h>

class CZ::CZPointerPinchEndEvent : public CZInputEvent
{
public:
    CZ_EVENT_DECLARE_COPY
    CZPointerPinchEndEvent() noexcept : CZInputEvent(Type::PointerPinchEnd) {};
    UInt32 fingers {};
    bool cancelled {};
};

#endif // CZ_CZPOINTERPINCHENDEVENT_H
