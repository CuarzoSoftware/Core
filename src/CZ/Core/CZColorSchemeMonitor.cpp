#include <CZ/Core/CZColorSchemeMonitor.h>
#include <CZ/Core/CZBus.h>
#include <systemd/sd-bus.h>
#include <cstring>

using namespace CZ;

static constexpr const char *Portal { "org.freedesktop.portal.Desktop" };
static constexpr const char *Path { "/org/freedesktop/portal/desktop" };
static constexpr const char *Iface { "org.freedesktop.portal.Settings" };
static constexpr const char *Namespace { "org.freedesktop.appearance" };
static constexpr const char *Key { "color-scheme" };

static std::weak_ptr<CZColorSchemeMonitor> s_monitor;

// color-scheme: 0 no preference, 1 prefer dark, 2 prefer light
static CZColorScheme ReadValue(sd_bus_message *m) noexcept
{
    UInt32 value { 0 };

    // A variant, possibly nested in another one (ReadOne vs the older Read)
    if (sd_bus_message_enter_container(m, 'v', "u") >= 0)
    {
        sd_bus_message_read(m, "u", &value);
        sd_bus_message_exit_container(m);
    }
    else if (sd_bus_message_enter_container(m, 'v', "v") >= 0 && sd_bus_message_enter_container(m, 'v', "u") >= 0)
        sd_bus_message_read(m, "u", &value);

    return value == 1 ? CZColorScheme::Dark : CZColorScheme::Light;
}

std::shared_ptr<CZColorSchemeMonitor> CZColorSchemeMonitor::GetOrMake() noexcept
{
    if (auto monitor { s_monitor.lock() })
        return monitor;

    auto bus { CZBus::GetOrMakeUser() };

    if (!bus)
        return {};

    std::shared_ptr<CZColorSchemeMonitor> monitor { new CZColorSchemeMonitor(bus) };
    s_monitor = monitor;
    return monitor;
}

CZColorSchemeMonitor::CZColorSchemeMonitor(std::shared_ptr<CZBus> bus) noexcept : m_bus(bus)
{
    // Changes...
    sd_bus_match_signal_async(m_bus->bus(), &m_signalSlot, nullptr, Path, Iface, "SettingChanged",
        [](sd_bus_message *m, void *data, sd_bus_error *) -> int {
            const char *ns {}, *key {};

            if (sd_bus_message_read(m, "ss", &ns, &key) >= 0 && ns && key &&
                strcmp(ns, Namespace) == 0 && strcmp(key, Key) == 0)
                static_cast<CZColorSchemeMonitor*>(data)->set(ReadValue(m));

            return 0;
        }, nullptr, this);

    // ...and the current value (async: the portal may need to be started)
    sd_bus_call_method_async(m_bus->bus(), &m_readSlot, Portal, Path, Iface, "ReadOne",
        [](sd_bus_message *m, void *data, sd_bus_error *) -> int {
            auto *monitor { static_cast<CZColorSchemeMonitor*>(data) };

            if (!sd_bus_message_is_method_error(m, nullptr))
                monitor->set(ReadValue(m));

            return 0;
        }, this, "ss", Namespace, Key);
}

CZColorSchemeMonitor::~CZColorSchemeMonitor() noexcept
{
    sd_bus_slot_unref(m_signalSlot);
    sd_bus_slot_unref(m_readSlot);
    notifyDestruction();
}

void CZColorSchemeMonitor::set(CZColorScheme scheme) noexcept
{
    if (scheme == CZColorScheme::Unknown)
        scheme = CZColorScheme::Light;

    m_known = true;

    if (m_colorScheme == scheme)
        return;

    m_colorScheme = scheme;
    onChanged.notify(scheme);
}
