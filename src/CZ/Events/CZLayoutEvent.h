#ifndef CZ_CZLAYOUTEVENT_H
#define CZ_CZLAYOUTEVENT_H

#include <CZ/Events/CZEvent.h>
#include <CZ/CZLayoutChange.h>
#include <CZ/CZBitset.h>

class CZ::CZLayoutEvent : public CZEvent
{
public:
    CZ_EVENT_DECLARE_COPY
    CZLayoutEvent(CZBitset<CZLayoutChange> changes = 0) noexcept :
        CZEvent(Type::Layout),
        changes(changes) {};
    CZBitset<CZLayoutChange> changes;
};

#endif // CZ_CZLAYOUTEVENT_H
