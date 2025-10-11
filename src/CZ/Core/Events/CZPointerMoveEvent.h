#ifndef CZ_CZPOINTERMOVEEVENT_H
#define CZ_CZPOINTERMOVEEVENT_H

#include <CZ/skia/core/SkPoint.h>
#include <CZ/Core/Events/CZInputEvent.h>

class CZ::CZPointerMoveEvent : public CZInputEvent
{
public:
    CZ_EVENT_DECLARE_COPY
    CZPointerMoveEvent() noexcept : CZInputEvent(Type::PointerMove) {}
    // Can be local or global depending on the context
    mutable SkPoint pos {};
    SkPoint delta {};
    SkPoint deltaUnaccelerated {};
};

#endif // CZ_CZPOINTERMOVEEVENT_H
