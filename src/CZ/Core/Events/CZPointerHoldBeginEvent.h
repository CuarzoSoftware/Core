#ifndef CZ_CZPOINTERHOLDBEGINEVENT_H
#define CZ_CZPOINTERHOLDBEGINEVENT_H

#include <CZ/Core/Events/CZInputEvent.h>
#include <CZ/skia/core/SkPoint.h>

class CZ::CZPointerHoldBeginEvent : public CZInputEvent
{
public:
    CZ_EVENT_DECLARE_COPY
    CZPointerHoldBeginEvent() noexcept : CZInputEvent(Type::PointerHoldBegin) {};
    UInt32 fingers {};
};

#endif // CZ_CZPOINTERHOLDBEGINEVENT_H
