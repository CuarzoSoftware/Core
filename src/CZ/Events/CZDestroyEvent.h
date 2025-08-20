#ifndef CZ_CZDESTROYEVENT_H
#define CZ_CZDESTROYEVENT_H

#include <CZ/Events/CZEvent.h>

class CZ::CZDestroyEvent : public CZEvent
{
public:
    CZ_EVENT_DECLARE_COPY
    CZDestroyEvent() noexcept : CZEvent(Type::Destroy) {};
};

#endif // CZ_CZDESTROYEVENT_H
