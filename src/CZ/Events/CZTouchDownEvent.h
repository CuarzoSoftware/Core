#ifndef CZ_CZTOUCHDOWNEVENT_H
#define CZ_CZTOUCHDOWNEVENT_H

#include <CZ/Events/CZInputEvent.h>
#include <CZ/skia/core/SkPoint.h>

class CZ::CZTouchDownEvent : public CZInputEvent
{
public:
    CZ_EVENT_DECLARE_COPY
    CZTouchDownEvent() noexcept : CZInputEvent(Type::TouchDown) {};

    mutable SkPoint localPos {};
    SkPoint pos {};
    Int32 id {};
};

#endif // CZ_CZTOUCHDOWNEVENT_H
