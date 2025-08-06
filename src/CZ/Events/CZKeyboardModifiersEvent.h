#ifndef CZ_CZKEYBOARDMODIFIERSEVENT_H
#define CZ_CZKEYBOARDMODIFIERSEVENT_H

#include <CZ/Events/CZInputEvent.h>

namespace CZ
{
    /**
     * @brief Keyboard modifiers.
     */
    struct CZKeyboardModifiers
    {
        /// Active modifiers when physically pressed
        UInt32 depressed = 0;

        /// Hooked modifiers that will be disabled after a non-modifier key is pressed
        UInt32 latched = 0;

        /// Active modifiers until they are pressed again (e.g. the Shift key)
        UInt32 locked = 0;

        /// Group the above states (use this value if the source of a modifier change is not of your interest)
        UInt32 group = 0;
    };
}

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
    CZKeyboardModifiers modifiers {};
};

#endif // CZ_CZKEYBOARDMODIFIERSEVENT_H
