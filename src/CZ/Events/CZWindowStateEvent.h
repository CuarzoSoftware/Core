#ifndef CZ_CZWINDOWSTATEEVENT_H
#define CZ_CZWINDOWSTATEEVENT_H

#include <CZ/Events/CZEvent.h>
#include <CZ/CZWindowState.h>
#include <CZ/CZBitset.h>

class CZ::CZWindowStateEvent : public CZEvent
{
public:
    CZ_EVENT_DECLARE_COPY
    CZWindowStateEvent() noexcept : CZEvent(Type::WindowState) {};
    CZBitset<CZWindowState> newState;
    CZBitset<CZWindowState> changes;
};

#endif // CZ_CZWINDOWSTATEEVENT_H
