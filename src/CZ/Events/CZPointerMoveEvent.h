#ifndef CZ_CZPOINTERMOVEEVENT_H
#define CZ_CZPOINTERMOVEEVENT_H

#include <CZ/skia/core/SkPoint.h>
#include <CZ/Events/CZInputEvent.h>

class CZ::CZPointerMoveEvent : public CZInputEvent
{
public:
    CZ_EVENT_DECLARE_COPY
    CZPointerMoveEvent() noexcept : CZInputEvent(Type::PointerMove) {}
    mutable SkPoint localPos {};
    SkPoint delta {};
    SkPoint deltaUnaccelerated {};
};

#endif // CZ_CZPOINTERMOVEEVENT_H
