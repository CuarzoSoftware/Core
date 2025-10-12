#include <CZ/Core/Events/CZKeyboardKeyEvent.h>
#include <CZ/Core/CZSharedMemory.h>
#include <CZ/Core/CZKeymap.h>
#include <CZ/Core/CZLog.h>
#include <cstring>

using namespace CZ;

std::shared_ptr<CZKeymap> CZKeymap::MakeServer(const xkb_rule_names &names) noexcept
{
    auto *context { xkb_context_new(XKB_CONTEXT_NO_FLAGS) };

    if (!context)
    {
        CZLog(CZError, CZLN, "Failed to create xkb_context");
        return {};
    }

    auto *keymap { xkb_keymap_new_from_names(context, &names, XKB_KEYMAP_COMPILE_NO_FLAGS) };

    if (!keymap)
    {
        CZLog(CZError, CZLN, "Failed to create xkb_keymap");
        xkb_context_unref(context);
        return {};
    }

    auto *state { xkb_state_new(keymap) };

    if (!state)
    {
        CZLog(CZError, CZLN, "Failed to create xkb_state");
        xkb_keymap_unref(keymap);
        xkb_context_unref(context);
        return {};
    }

    auto *str { xkb_keymap_get_as_string(keymap, XKB_KEYMAP_FORMAT_TEXT_V1) };

    if (!str)
    {
        CZLog(CZError, CZLN, "Failed to get xkb_keymap as string");
        xkb_state_unref(state);
        xkb_keymap_unref(keymap);
        xkb_context_unref(context);
        return {};
    }

    const auto strSize { strlen(str) };
    auto shm { CZSharedMemory::Make(strSize, O_RDWR | O_CREAT | O_EXCL | O_CLOEXEC, 0600) };

    if (!shm)
    {
        CZLog(CZError, CZLN, "Failed to create shm");
        free(str);
        xkb_state_unref(state);
        xkb_keymap_unref(keymap);
        xkb_context_unref(context);
        return {};
    }

    memcpy(shm->map(), str, strSize);

    auto roFd { shm_open(shm->name().c_str(), O_RDONLY | O_CLOEXEC, 0600) };

    if (roFd < 0)
    {
        CZLog(CZError, CZLN, "Failed to create read only fd");
        free(str);
        xkb_state_unref(state);
        xkb_keymap_unref(keymap);
        xkb_context_unref(context);
        return {};
    }

    return std::shared_ptr<CZKeymap>(new CZKeymap(context, keymap, state, roFd, strSize, false));
}

std::shared_ptr<CZ::CZKeymap> CZKeymap::MakeClient(int fd, size_t size) noexcept
{
    const char *map { (const char*)mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0) };

    if (map == MAP_FAILED)
    {
        CZLog(CZError, CZLN, "Failed to map server keymap");
        return {};
    }

    auto *context { xkb_context_new(XKB_CONTEXT_NO_FLAGS) };

    if (!context)
    {
        CZLog(CZError, CZLN, "Failed to create xkb_context");
        munmap((void*)map, size);
        return {};
    }

    auto *keymap { xkb_keymap_new_from_string(context, map, XKB_KEYMAP_FORMAT_TEXT_V1, XKB_KEYMAP_COMPILE_NO_FLAGS) };
    munmap((void*)map, size);

    if (!keymap)
    {
        CZLog(CZError, CZLN, "Failed to create xkb_keymap");
        xkb_context_unref(context);
        return {};
    }

    auto *state { xkb_state_new(keymap) };

    if (!state)
    {
        CZLog(CZError, CZLN, "Failed to create xkb_state");
        xkb_keymap_unref(keymap);
        xkb_context_unref(context);
        return {};
    }

    return std::shared_ptr<CZKeymap>(new CZKeymap(context, keymap, state, -1, 0, true));
}

CZKeymap::~CZKeymap() noexcept
{
    if (m_fd >= 0)
        close(m_fd);

    if (m_composeState)
        xkb_compose_state_unref(m_composeState);

    if (m_composeTable)
        xkb_compose_table_unref(m_composeTable);

    xkb_state_unref(m_state);
    xkb_keymap_unref(m_keymap);
    xkb_context_unref(m_context);
}

void CZKeymap::updateModifiers(UInt32 depressed, UInt32 latched, UInt32 locked, UInt32 group) noexcept
{
    if (isClient())
    {
        xkb_state_update_mask(m_state, depressed, latched, locked, 0, 0, group);
        m_modifiers.depressed = depressed;
        m_modifiers.latched = latched;
        m_modifiers.locked = locked;
        m_modifiers.group = group;
    }
}

xkb_compose_status CZKeymap::composeStatus() const noexcept
{
    if (!m_composeState)
        return XKB_COMPOSE_NOTHING;

    return xkb_compose_state_get_status(m_composeState);
}

void CZKeymap::feed(const CZKeyboardKeyEvent &e) noexcept
{
    static char buffer[128];
    const auto code { e.code + 8 };

    if (e.isPressed)
    {
        const auto res { m_pressedKeys.insert(e.code) };

        if (!res.second)
        {
            CZLog(CZWarning, CZLN, "Key code {} already pressed. Ignoring event...", e.code);
            return;
        }

        if (!isClient())
            xkb_state_update_key(m_state, code, XKB_KEY_DOWN);

        e.symbol = xkb_state_key_get_one_sym(m_state, code);

        if (m_composeState)
        {
            xkb_compose_state_feed(m_composeState, e.symbol);
        }

        e.composeStatus = composeStatus();

        switch (e.composeStatus)
        {
        case XKB_COMPOSE_NOTHING:
            xkb_state_key_get_utf8(m_state, code, buffer, sizeof(buffer));
            e.utf8 = buffer;
            break;
        case XKB_COMPOSE_COMPOSED:
            xkb_compose_state_get_utf8(m_composeState, buffer, sizeof(buffer));
            xkb_compose_state_reset(m_composeState);
            e.utf8 = buffer;
            break;
        case XKB_COMPOSE_COMPOSING:
        case XKB_COMPOSE_CANCELLED:
            e.utf8 = "";
        }
    }
    else
    {
        auto it { m_pressedKeys.find(e.code) };

        if (it == m_pressedKeys.end())
        {
            CZLog(CZWarning, CZLN, "Key code {} already released. Ignoring event...", e.code);
            return;
        }

        m_pressedKeys.erase(it);

        if (!isClient())
            xkb_state_update_key(m_state, code, XKB_KEY_UP);

        e.symbol = xkb_state_key_get_one_sym(m_state, code);
        e.composeStatus = composeStatus();
        xkb_state_key_get_utf8(m_state, code, buffer, sizeof(buffer));
        e.utf8 = buffer;
    }

    m_modifiers.depressed = xkb_state_serialize_mods(state(), XKB_STATE_MODS_DEPRESSED);
    m_modifiers.latched = xkb_state_serialize_mods(state(), XKB_STATE_MODS_LATCHED);
    m_modifiers.locked = xkb_state_serialize_mods(state(), XKB_STATE_MODS_LOCKED);
    m_modifiers.group = xkb_state_serialize_layout(state(), XKB_STATE_LAYOUT_EFFECTIVE);

    CZLog(CZTrace, "Key Code: {}, UTF8: {}, State: {}", e.code, e.utf8, e.isPressed ? "Pressed" : "Released");
}

void CZKeymap::setRepeatInfo(Int32 delayMs, Int32 rateMs) noexcept
{
    if (delayMs < 0) delayMs = 0;
    if (rateMs < 0) rateMs = 0;

    if (m_repeatDelayMs != delayMs || m_repeatRateMs != rateMs)
    {
        m_repeatDelayMs = delayMs;
        m_repeatRateMs = rateMs;
        onRepeatInfoChanged.notify();
    }
}

static std::string localePostDotToUpper(const std::string &locale)
{
    size_t lastDotPos = locale.find_last_of('.');

    // If no dot is found, return the original string
    if (lastDotPos == std::string::npos)
        return locale;

    std::string result { locale };

    // Convert everything after the last dot to uppercase
    std::transform(result.begin() + lastDotPos + 1, result.end(), result.begin() + lastDotPos + 1, ::toupper);

    return result;
}

void CZKeymap::loadComposeTable(const char *locale) noexcept
{
    if (m_composeState)
    {
        xkb_compose_state_unref(m_composeState);
        m_composeState = nullptr;
    }

    if (m_composeTable)
    {
        xkb_compose_table_unref(m_composeTable);
        m_composeTable = nullptr;
    }

    if (!locale || !*locale)
        locale = getenv("LC_ALL");
    if (!locale || !*locale)
        locale = getenv("LC_CTYPE");
    if (!locale || !*locale)
        locale = getenv("LANG");
    if (!locale || !*locale)
        locale = "C";

    m_composeTable = xkb_compose_table_new_from_locale(m_context, locale, XKB_COMPOSE_COMPILE_NO_FLAGS);

    if (!m_composeTable)
        goto fail;

    m_composeState = xkb_compose_state_new(m_composeTable, XKB_COMPOSE_STATE_NO_FLAGS);

    if (!m_composeState)
    {
        xkb_compose_table_unref(m_composeTable);
        m_composeTable = nullptr;
        goto fail;
    }
    CZLog(CZInfo, CZLN, "Using locale {}", locale);
    return;
fail:
    // Try with uppercase
    if (locale)
    {
        const std::string localeNormal { locale };
        const std::string localeUpper { localePostDotToUpper(localeNormal) };

        if (localeNormal != localeUpper)
        {
            CZLog(CZWarning, CZLN, "Failed to create compose table from locale {}. Trying with {}", locale, localeUpper.c_str());
            return loadComposeTable(localeUpper.c_str());
        }
    }

    // Fallback
    if (strcmp(locale, "C") != 0)
    {
        CZLog(CZWarning, CZLN, "Failed to create compose table from locale {}. Falling back to locale C", locale);
        loadComposeTable("C");
    }
}
