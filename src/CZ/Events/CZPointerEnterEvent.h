#ifndef CZ_CZPOINTERENTEREVENT_H
#define CZ_CZPOINTERENTEREVENT_H

#include <CZ/Events/CZInputEvent.h>
#include <CZ/CZTime.h>
#include <CZ/skia/core/SkPoint.h>

class CZ::CZPointerEnterEvent : public CZInputEvent
{
public:
    CZ_EVENT_DECLARE_COPY
    CZPointerEnterEvent() noexcept : CZInputEvent(Type::PointerEnter) {};
    mutable SkPoint localPos {};
};

#endif // CZ_CZPOINTERENTEREVENT_H
