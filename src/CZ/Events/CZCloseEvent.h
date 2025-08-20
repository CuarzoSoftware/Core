#ifndef CZ_CZCLOSEEVENT_H
#define CZ_CZCLOSEEVENT_H

#include <CZ/Events/CZEvent.h>

class CZ::CZCloseEvent : public CZEvent
{
public:
    CZ_EVENT_DECLARE_COPY
    CZCloseEvent() noexcept : CZEvent(Type::Close) {};
};

#endif // CZ_CZCLOSEEVENT_H
