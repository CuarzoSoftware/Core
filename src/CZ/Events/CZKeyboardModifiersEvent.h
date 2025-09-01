#ifndef CZ_CZKEYBOARDMODIFIERSEVENT_H
#define CZ_CZKEYBOARDMODIFIERSEVENT_H

#include <CZ/Events/CZInputEvent.h>
#include <CZ/Core/CZKeyModifiers.h>

/**
 * @brief Keyboard modifiers event.
 *
 * Keyboard modifiers events are automatically sent to client surfaces when they acquire keyboard focus.
 */
class CZ::CZKeyboardModifiersEvent : public CZInputEvent
{
public:
    CZ_EVENT_DECLARE_COPY
    CZKeyboardModifiersEvent() noexcept : CZInputEvent(Type::KeyboardModifiers) {};
    CZKeyModifiers modifiers {};
};

#endif // CZ_CZKEYBOARDMODIFIERSEVENT_H
