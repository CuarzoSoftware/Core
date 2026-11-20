#ifndef CZ_CZKEYBOARDKEYEVENT_H
#define CZ_CZKEYBOARDKEYEVENT_H

#include <CZ/Core/Events/CZInputEvent.h>
#include <xkbcommon/xkbcommon-compose.h>

/**
 * @brief A keyboard key was pressed or released.
 *
 * Backends fill @ref code, @ref isPressed and the timestamps. The keymap (CZKeymap) then fills
 * @ref symbol, @ref composeStatus and @ref utf8 according to the current keyboard layout and state.
 */
class CZ::CZKeyboardKeyEvent : public CZInputEvent
{
public:
    CZ_EVENT_DECLARE_COPY
    CZKeyboardKeyEvent() noexcept : CZInputEvent(Type::KeyboardKey) {};

    /** @brief `true` if the key was pressed, `false` if released. */
    mutable bool isPressed {};
    /** @brief `true` if the event is a key repeat generated while the key is held down (e.g. by Kay). */
    mutable bool isRepeat {};

    /** @brief Raw key code (evdev), see `<linux/input-event-codes.h>`. */
    mutable UInt32 code {};
    /** @brief XKB key symbol (e.g. `XKB_KEY_Return`) for the current layout and modifiers. */
    mutable xkb_keysym_t symbol {};

    /** @brief Status of the compose (dead key) sequence after this key. */
    mutable xkb_compose_status composeStatus { XKB_COMPOSE_NOTHING };
    /**
     * @brief Text produced by the key, UTF-8 encoded.
     *
     * If a compose sequence was just completed it contains the composed text, and it is empty
     * while composing or if the sequence was cancelled.
     */
    mutable std::string utf8 {};
};

#endif // CZ_CZKEYBOARDKEYEVENT_H
