#ifndef CZ_CZPOINTERSWIPEBEGIN_H
#define CZ_CZPOINTERSWIPEBEGIN_H

#include <CZ/Core/Events/CZInputEvent.h>
#include <CZ/skia/core/SkPoint.h>

class CZ::CZPointerSwipeBeginEvent : public CZInputEvent
{
public:
    CZ_EVENT_DECLARE_COPY
    CZPointerSwipeBeginEvent() noexcept : CZInputEvent(Type::PointerSwipeBegin) {};
    UInt32 fingers {};
};

#endif // CZ_CZPOINTERSWIPEBEGIN_H
