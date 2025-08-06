#ifndef CZ_CZTOUCHMOVEEVENT_H
#define CZ_CZTOUCHMOVEEVENT_H

#include <CZ/Events/CZInputEvent.h>
#include <CZ/skia/core/SkPoint.h>

class CZ::CZTouchMoveEvent : public CZInputEvent
{
public:
    CZ_EVENT_DECLARE_COPY
    CZTouchMoveEvent() noexcept : CZInputEvent(Type::TouchMove) {};
    mutable SkPoint localPos {};
    Int32 id {};
    SkPoint pos {};
};

#endif // CZ_CZTOUCHMOVEEVENT_H
