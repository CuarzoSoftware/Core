#ifndef CZ_CZCLOSEEVENT_H
#define CZ_CZCLOSEEVENT_H

#include <CZ/Core/Events/CZEvent.h>

/**
 * @brief Request to close an object (e.g. a window).
 *
 * Sent, for example, to a Marco MToplevel when the compositor or the window's close button asks it to close.
 * The receiver can refuse the request by calling ignore() on the event.
 */
class CZ::CZCloseEvent : public CZEvent
{
public:
    CZ_EVENT_DECLARE_COPY
    CZCloseEvent() noexcept : CZEvent(Type::Close) {};
};

#endif // CZ_CZCLOSEEVENT_H
