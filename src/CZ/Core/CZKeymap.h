#ifndef CZ_CZKEYMAP_H
#define CZ_CZKEYMAP_H

#include <CZ/Core/CZObject.h>
#include <xkbcommon/xkbcommon.h>
#include <memory>

class CZ::CZKeymap : public CZObject
{
public:
    static std::shared_ptr<CZKeymap> MakeServer(const xkb_rule_names &names) noexcept;
    ~CZKeymap() noexcept;

    xkb_context *context() const noexcept { return m_context; }
    xkb_keymap *keymap() const noexcept { return m_keymap; }
    xkb_state *state() const noexcept { return m_state; }

    // Read-only fd to share to Wayland clients (valid only if created with MakeServer())
    int fd() const noexcept { return m_fd; }

    // Size of the keymap string in bytes (of the fd()) (valid only if created with MakeServer())
    size_t size() const noexcept { return m_size; }

private:
    CZKeymap(xkb_context *context, xkb_keymap *keymap, xkb_state *state, int fd, size_t size) noexcept :
        m_context(context), m_keymap(keymap), m_state(state), m_fd(fd), m_size(size) {}

    xkb_context *m_context;
    xkb_keymap *m_keymap;
    xkb_state *m_state;
    int m_fd;
    size_t m_size;
};

#endif // CZKEYMAP_H
