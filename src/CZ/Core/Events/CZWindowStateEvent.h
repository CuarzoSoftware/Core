#ifndef CZ_CZWINDOWSTATEEVENT_H
#define CZ_CZWINDOWSTATEEVENT_H

#include <CZ/Core/Events/CZEvent.h>
#include <CZ/Core/CZWindowState.h>
#include <CZ/Core/CZBitset.h>

/**
 * @brief Notifies that the state of a window changed (activated, maximized, fullscreen, etc.).
 *
 * Sent, for example, by Marco to a MToplevel when the compositor configures a new state. Kay forwards it to the window's scene and nodes.
 */
class CZ::CZWindowStateEvent : public CZEvent
{
public:
    CZ_EVENT_DECLARE_COPY
    CZWindowStateEvent() noexcept : CZEvent(Type::WindowState) {};
    /** @brief The new window state flags. */
    CZBitset<CZWindowState> newState;
    /** @brief The flags that changed compared to the previous state. */
    CZBitset<CZWindowState> changes;
};

#endif // CZ_CZWINDOWSTATEEVENT_H
