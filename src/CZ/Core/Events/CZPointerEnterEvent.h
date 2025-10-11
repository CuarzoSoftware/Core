#ifndef CZ_CZPOINTERENTEREVENT_H
#define CZ_CZPOINTERENTEREVENT_H

#include <CZ/Core/Events/CZInputEvent.h>
#include <CZ/Core/CZTime.h>
#include <CZ/skia/core/SkPoint.h>

class CZ::CZPointerEnterEvent : public CZInputEvent
{
public:
    CZ_EVENT_DECLARE_COPY
    CZPointerEnterEvent() noexcept : CZInputEvent(Type::PointerEnter) {};
    // Can be local or global depending on the context
    mutable SkPoint pos {};
};

#endif // CZ_CZPOINTERENTEREVENT_H
