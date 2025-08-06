#ifndef CZ_CZPOINTERSWIPEUPDATE_H
#define CZ_CZPOINTERSWIPEUPDATE_H

#include <CZ/Events/CZInputEvent.h>
#include <CZ/skia/core/SkPoint.h>

class CZ::CZPointerSwipeUpdateEvent : public CZInputEvent
{
public:
    CZ_EVENT_DECLARE_COPY
    CZPointerSwipeUpdateEvent() noexcept : CZInputEvent(Type::PointerSwipeUpdate) {};
    UInt32 fingers {};
    SkPoint delta {};
    SkPoint deltaUnaccelerated {};
};

#endif // CZ_CZPOINTERSWIPEUPDATE_H
