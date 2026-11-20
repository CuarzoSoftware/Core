#ifndef CZ_CZKEYMAP_H
#define CZ_CZKEYMAP_H

#include <CZ/Core/CZObject.h>
#include <CZ/Core/CZKeyModifiers.h>
#include <unordered_set>
#include <xkbcommon/xkbcommon.h>
#include <xkbcommon/xkbcommon-compose.h>
#include <memory>

/**
 * @brief XKB keymap and keyboard state.
 *
 * Wraps an xkb_context, xkb_keymap and xkb_state, plus an optional compose table/state (loaded from
 * the `LC_ALL`, `LC_CTYPE` or `LANG` locale, falling back to "C"). Key events passed to feed() are
 * translated into keysyms and UTF-8 text, and the modifiers state is tracked.
 *
 * - Server keymaps (MakeServer()) compute their own state from key presses and expose the keymap
 *   string through a read-only fd() / size() pair that can be shared with Wayland clients.
 * - Client keymaps (MakeClient()) are built from a keymap received from a server, and their modifiers
 *   are set by the server through updateModifiers().
 *
 * The current keymap used by the process is held by CZCore (see CZCore::keymap()).
 */
class CZ::CZKeymap : public CZObject
{
public:
    /** @brief Compiles a server keymap from XKB rule names. Returns `nullptr` on failure. */
    static std::shared_ptr<CZKeymap> MakeServer(const xkb_rule_names &names) noexcept;

    /**
     * @brief Creates a client keymap from a keymap string shared by a server (e.g. Wayland `wl_keyboard.keymap`).
     *
     * Does not take ownership of @p fd. Returns `nullptr` on failure.
     *
     * @param fd File descriptor of the keymap string, mapped read-only.
     * @param size Size of the keymap string in bytes.
     */
    static std::shared_ptr<CZKeymap> MakeClient(int fd, size_t size) noexcept; // Does not take ownership

    /** @brief Whether the keymap was created with MakeClient(). */
    bool isClient() const noexcept { return m_isClient; }
    ~CZKeymap() noexcept;

    /** @brief Keycodes currently pressed, as tracked by feed(). */
    const std::unordered_set<UInt32> &pressedKeys() const noexcept { return m_pressedKeys; }

    // These always exist
    /** @brief The XKB context (never `nullptr`). */
    xkb_context *context() const noexcept { return m_context; }

    /** @brief The XKB keymap (never `nullptr`). */
    xkb_keymap *keymap() const noexcept { return m_keymap; }

    /** @brief The XKB state (never `nullptr`). */
    xkb_state *state() const noexcept { return m_state; }

    /** @brief Current modifiers state, updated by feed() and updateModifiers(). */
    const CZKeyModifiers &modifiers() const noexcept { return m_modifiers; }

    /**
     * @brief Updates the XKB state and modifiers with values sent by the server.
     *
     * Only has an effect on client keymaps.
     */
    void updateModifiers(UInt32 depressed, UInt32 latched, UInt32 locked, UInt32 group) noexcept;

    /** @brief The compose table, or `nullptr` if it could not be loaded. */
    xkb_compose_table *composeTable() const noexcept { return m_composeTable; }

    /** @brief The compose state, or `nullptr` if there is no compose table. */
    xkb_compose_state *composeState() const noexcept { return m_composeState; }

    /** @brief Current compose status. Always `XKB_COMPOSE_NOTHING` if there is no compose state/table. */
    xkb_compose_status composeStatus() const noexcept;

    /**
     * @brief Processes a key press or release.
     *
     * Updates pressedKeys(), the XKB state (server keymaps only), the compose state and modifiers(),
     * and fills the event's `symbol`, `composeStatus` and `utf8` fields. Presses of already pressed
     * keys and releases of keys not pressed are ignored.
     */
    void feed(const CZKeyboardKeyEvent &e) noexcept;

    /**
     * @brief Sets the key repeat delay and rate, emitting @ref onRepeatInfoChanged if they change.
     *
     * Negative values are clamped to 0. A rate of 0 ms disables repeat.
     */
    void setRepeatInfo(Int32 delayMs, Int32 rateMs) noexcept;

    /** @brief Key repeat rate in milliseconds, 0 if repeat is disabled (default 32). */
    Int32 repeatRateMs() const noexcept { return m_repeatRateMs; }

    /** @brief Delay in milliseconds before a held key starts repeating (default 500). */
    Int32 repeatDelayMs() const noexcept { return m_repeatDelayMs; }

    /** @brief Emitted when setRepeatInfo() changes the repeat delay or rate. */
    CZSignal<> onRepeatInfoChanged;

    /** @brief Read-only fd of the keymap string, to share with Wayland clients. Valid only if created with MakeServer(), -1 otherwise. */
    int fd() const noexcept { return m_fd; }

    /** @brief Size in bytes of the keymap string in fd(). Valid only if created with MakeServer(), 0 otherwise. */
    size_t size() const noexcept { return m_size; }

private:
    CZKeymap(xkb_context *context, xkb_keymap *keymap, xkb_state *state, int fd, size_t size, bool isClient) noexcept :
        m_context(context), m_keymap(keymap), m_state(state), m_fd(fd), m_size(size), m_isClient(isClient) {
        loadComposeTable();
    }
    void loadComposeTable(const char *locale = nullptr) noexcept;
    std::unordered_set<UInt32> m_pressedKeys;
    xkb_context *m_context;
    xkb_keymap *m_keymap;
    xkb_state *m_state;
    CZKeyModifiers m_modifiers {};
    xkb_compose_table *m_composeTable {};
    xkb_compose_state *m_composeState {};
    Int32 m_repeatRateMs { 32 };
    Int32 m_repeatDelayMs { 500 };
    int m_fd;
    size_t m_size;
    bool m_isClient;
};

#endif // CZKEYMAP_H
