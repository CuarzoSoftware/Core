#ifndef CZ_CZPOINTERLEAVEEVENT_H
#define CZ_CZPOINTERLEAVEEVENT_H

#include <CZ/skia/core/SkPoint.h>
#include <CZ/Events/CZInputEvent.h>

class CZ::CZPointerLeaveEvent : public CZInputEvent
{
public:
    CZ_EVENT_DECLARE_COPY
    CZPointerLeaveEvent() noexcept : CZInputEvent(Type::PointerLeave) {};
};

#endif // CZ_CZPOINTERLEAVEEVENT_H
