#ifndef CZ_CZPOINTERHOLDENDEVENT_H
#define CZ_CZPOINTERHOLDENDEVENT_H

#include <CZ/Core/Events/CZInputEvent.h>
#include <CZ/skia/core/SkPoint.h>

class CZ::CZPointerHoldEndEvent : public CZInputEvent
{
public:
    CZ_EVENT_DECLARE_COPY
    CZPointerHoldEndEvent() noexcept : CZInputEvent(Type::PointerHoldEnd) {};
    UInt32 fingers {};
    bool cancelled {};
};

#endif // CZ_CZPOINTERHOLDENDEVENT_H
