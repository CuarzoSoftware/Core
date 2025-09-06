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

    return std::shared_ptr<CZKeymap>(new CZKeymap(context, keymap, state, roFd, strSize));
}

CZKeymap::~CZKeymap() noexcept
{
    if (m_fd >= 0)
        close(m_fd);

    xkb_state_unref(m_state);
    xkb_keymap_unref(m_keymap);
    xkb_context_unref(m_context);
}
