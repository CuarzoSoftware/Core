#include <CZ/Core/CZEventSource.h>
#include <CZ/Core/CZBus.h>
#include <CZ/Core/CZLog.h>
#include <systemd/sd-bus.h>

using namespace CZ;

static std::weak_ptr<CZBus> s_system;
static std::weak_ptr<CZBus> s_user;

std::shared_ptr<CZBus> CZBus::GetOrMakeSystem() noexcept
{
    if (auto bus = s_system.lock())
        return bus;

    sd_bus *bus {};
    int r;

    r = sd_bus_open_system(&bus);

    if (r < 0)
    {
        CZLog(CZError, CZLN, "Failed to connect to system bus: {}", strerror(-r));
        return {};
    }

    auto eventSource { CZEventSource::Make(
        sd_bus_get_fd(bus),
        sd_bus_get_events(bus),
        CZOwn::Borrow,
        [bus](auto, auto, CZEventSource *source){
            int r;

            while ((r = sd_bus_process(bus, NULL)) > 0);

            if (r < 0)
                CZLog(CZError, CZLN, "sd_bus_process failed: {}", strerror(-r));

            source->setEvents(sd_bus_get_events(bus));
        })
    };

    if (!eventSource)
    {
        sd_bus_unref(bus);
        return {};
    }

    auto instance { std::shared_ptr<CZBus>(new CZBus(bus, Type::System, eventSource)) };
    s_system = instance;
    return instance;
}

std::shared_ptr<CZBus> CZBus::GetSystem() noexcept
{
    return s_system.lock();
}

std::shared_ptr<CZBus> CZBus::GetOrMakeUser() noexcept
{
    if (auto bus = s_user.lock())
        return bus;

    sd_bus *bus {};
    int r;

    r = sd_bus_open_user(&bus);

    if (r < 0)
    {
        CZLog(CZError, CZLN, "Failed to connect to user bus: {}", strerror(-r));
        return {};
    }

    auto eventSource { CZEventSource::Make(
        sd_bus_get_fd(bus),
        sd_bus_get_events(bus),
        CZOwn::Borrow,
        [bus](auto, auto, CZEventSource *source){
            int r;

            while ((r = sd_bus_process(bus, NULL)) > 0);

            if (r < 0)
                CZLog(CZError, CZLN, "sd_bus_process failed: {}", strerror(-r));

            source->setEvents(sd_bus_get_events(bus));
        })
    };

    if (!eventSource)
    {
        sd_bus_unref(bus);
        return {};
    }

    auto instance { std::shared_ptr<CZBus>(new CZBus(bus, Type::User, eventSource)) };
    s_user = instance;
    return instance;
}

CZBus::CZBus(sd_bus *bus, Type type, std::shared_ptr<CZEventSource> eventSource) noexcept :
    m_bus(bus), m_type(type), m_eventSource(eventSource) {}

std::shared_ptr<CZBus> CZBus::GetUser() noexcept
{
    return s_user.lock();
}

int CZBus::timeoutMS() const noexcept
{
    int r;
    UInt64 timeout;

    r = sd_bus_get_timeout(m_bus, &timeout);
    if (r < 0) {
        CZLog(CZError, CZLN, "Failed to connect to system bus: {}", strerror(-r));
        return -1;
    }

    if (timeout == UINT64_MAX)
        return -1;

    return (int)((timeout + 999) / 1000);
}

CZBus::~CZBus() noexcept
{
    sd_bus_unref(m_bus);
}

