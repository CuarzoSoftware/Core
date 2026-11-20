#ifndef CZ_CZLAYOUTEVENT_H
#define CZ_CZLAYOUTEVENT_H

#include <CZ/Core/Events/CZEvent.h>
#include <CZ/Core/CZLayoutChange.h>
#include <CZ/Core/CZBitset.h>

/**
 * @brief Notifies that the layout of an object changed.
 *
 * Kay sends it to nodes whose position, size or scale changed after a layout pass.
 */
class CZ::CZLayoutEvent : public CZEvent
{
public:
    CZ_EVENT_DECLARE_COPY
    /** @brief Creates the event with the given changes. */
    CZLayoutEvent(CZBitset<CZLayoutChange> changes = 0) noexcept :
        CZEvent(Type::Layout),
        changes(changes) {};
    /** @brief What changed (position, size and/or scale). */
    CZBitset<CZLayoutChange> changes;
};

#endif // CZ_CZLAYOUTEVENT_H
