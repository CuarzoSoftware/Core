#ifndef CZ_CZCOLORSCHEMEMONITOR_H
#define CZ_CZCOLORSCHEMEMONITOR_H

#include <CZ/Core/CZObject.h>
#include <CZ/Core/CZSignal.h>
#include <CZ/Core/CZColorScheme.h>
#include <memory>

struct sd_bus_slot;

/**
 * @brief Follows the desktop's light/dark preference.
 *
 * Reads the `color-scheme` setting of the `org.freedesktop.appearance` namespace from the XDG
 * settings portal (`org.freedesktop.portal.Settings`, on the session bus) and listens for its
 * changes. Desktops set it e.g. through GNOME's `org.gnome.desktop.interface color-scheme`
 * (Firmament's Dark Mode).
 *
 * The first value is read asynchronously (the portal may need to be started): until then
 * colorScheme() is Light and known() is `false`; onChanged is emitted once it's read if it differs.
 *
 * There is a single instance per process, shared through GetOrMake(). It requires CZCore (its D-Bus
 * connection is dispatched by CZCore's event loop).
 */
class CZ::CZColorSchemeMonitor final : public CZObject
{
public:
    /// The shared instance (created on first use). `nullptr` without a session bus.
    static std::shared_ptr<CZColorSchemeMonitor> GetOrMake() noexcept;

    ~CZColorSchemeMonitor() noexcept;

    /// The current preference (Light or Dark).
    CZColorScheme colorScheme() const noexcept { return m_colorScheme; }

    /// Whether it was read already (else colorScheme() is just the default).
    bool known() const noexcept { return m_known; }

    /// Emitted when the preference changes.
    CZSignal<CZColorScheme> onChanged;

private:
    CZColorSchemeMonitor(std::shared_ptr<CZBus> bus) noexcept;
    void set(CZColorScheme scheme) noexcept;
    std::shared_ptr<CZBus> m_bus;
    sd_bus_slot *m_signalSlot {}, *m_readSlot {};
    CZColorScheme m_colorScheme { CZColorScheme::Light };
    bool m_known { false };
};

#endif // CZ_CZCOLORSCHEMEMONITOR_H
