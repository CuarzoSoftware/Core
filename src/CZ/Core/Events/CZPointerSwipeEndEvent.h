#ifndef CZ_CZPOINTERSWIPEENDEVENT_H
#define CZ_CZPOINTERSWIPEENDEVENT_H

#include <CZ/Core/Events/CZInputEvent.h>
#include <CZ/skia/core/SkPoint.h>

class CZ::CZPointerSwipeEndEvent : public CZInputEvent
{
public:
    CZ_EVENT_DECLARE_COPY
    CZPointerSwipeEndEvent() noexcept : CZInputEvent(Type::PointerSwipeEnd) {};
    UInt32 fingers {};
    bool cancelled {};
};

#endif // CZ_CZPOINTERSWIPEENDEVENT_H
