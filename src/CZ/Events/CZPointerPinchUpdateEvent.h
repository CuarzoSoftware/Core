#ifndef CZ_CZPOINTERPINCHUPDATEEVENT_H
#define CZ_CZPOINTERPINCHUPDATEEVENT_H

#include <CZ/Events/CZInputEvent.h>
#include <CZ/skia/core/SkPoint.h>

class CZ::CZPointerPinchUpdateEvent : public CZInputEvent
{
public:
    CZ_EVENT_DECLARE_COPY
    CZPointerPinchUpdateEvent() noexcept : CZInputEvent(Type::PointerPinchUpdate) {};
    UInt32 fingers {};
    SkPoint delta {};
    SkPoint deltaUnaccelerated {};
    Float32 scale {};
    Float32 rotation {};
};

#endif // CZ_CZPOINTERPINCHUPDATEEVENT_H
