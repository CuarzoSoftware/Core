#ifndef CZ_CZCOLORSCHEMEEVENT_H
#define CZ_CZCOLORSCHEMEEVENT_H

#include <CZ/Core/Events/CZEvent.h>
#include <CZ/Core/CZColorScheme.h>

/**
 * @brief Notifies that the color scheme (dark/light) of an object changed.
 *
 * Kay sends it to nodes and scenes when their color scheme changes.
 */
class CZ::CZColorSchemeEvent : public CZEvent
{
public:
    CZ_EVENT_DECLARE_COPY
    /** @brief Creates the event with the new color scheme. */
    CZColorSchemeEvent(CZColorScheme scheme) noexcept : CZEvent(Type::ColorScheme), scheme(scheme) {};
    /** @brief The new color scheme. */
    CZColorScheme scheme;
};


#endif // CZ_CZCOLORSCHEMEEVENT_H
