#ifndef CZ_CZKEYMAP_H
#define CZ_CZKEYMAP_H

#include <CZ/Core/CZObject.h>
#include <CZ/Core/CZKeyModifiers.h>
#include <unordered_set>
#include <xkbcommon/xkbcommon.h>
#include <xkbcommon/xkbcommon-compose.h>
#include <memory>

class CZ::CZKeymap : public CZObject
{
public:
    static std::shared_ptr<CZKeymap> MakeServer(const xkb_rule_names &names) noexcept;
    static std::shared_ptr<CZKeymap> MakeClient(int fd, size_t size) noexcept; // Does not take ownership
    bool isClient() const noexcept { return m_isClient; }
    ~CZKeymap() noexcept;

    const std::unordered_set<UInt32> &pressedKeys() const noexcept { return m_pressedKeys; }

    // These always exist
    xkb_context *context() const noexcept { return m_context; }
    xkb_keymap *keymap() const noexcept { return m_keymap; }
    xkb_state *state() const noexcept { return m_state; }

    const CZKeyModifiers &modifiers() const noexcept { return m_modifiers; }

    // Only for clients
    void updateModifiers(UInt32 depressed, UInt32 latched, UInt32 locked, UInt32 group) noexcept;

    // These could be nullptr
    xkb_compose_table *composeTable() const noexcept { return m_composeTable; }
    xkb_compose_state *composeState() const noexcept { return m_composeState; }

    // Always returns XKB_COMPOSE_NOTHING if there is no compose state/table
    xkb_compose_status composeStatus() const noexcept;

    void feed(const CZKeyboardKeyEvent &e) noexcept;

    /**
     * A rate of 0 ms disables repeat.
     */
    void setRepeatInfo(Int32 delayMs, Int32 rateMs) noexcept;
    Int32 repeatRateMs() const noexcept { return m_repeatRateMs; }
    Int32 repeatDelayMs() const noexcept { return m_repeatDelayMs; }
    CZSignal<> onRepeatInfoChanged;

    // Read-only fd to share to Wayland clients (valid only if created with MakeServer())
    int fd() const noexcept { return m_fd; }

    // Size of the keymap string in bytes (of the fd()) (valid only if created with MakeServer())
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
