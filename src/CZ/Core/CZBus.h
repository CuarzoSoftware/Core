#ifndef CZBUS_H
#define CZBUS_H

#include <CZ/Core/CZObject.h>
#include <memory>

struct sd_bus;

class CZ::CZBus : public CZObject
{
public:
    enum class Type
    {
        System,
        User
    };

    ~CZBus() noexcept;

    [[nodiscard]] static std::shared_ptr<CZBus> GetOrMakeSystem() noexcept;
    static std::shared_ptr<CZBus> GetSystem() noexcept;

    [[nodiscard]] static std::shared_ptr<CZBus> GetOrMakeUser() noexcept;
    static std::shared_ptr<CZBus> GetUser() noexcept;

    std::shared_ptr<CZEventSource> eventSource() const noexcept { return m_eventSource; }
    int timeoutMS() const noexcept;
    sd_bus *bus() const noexcept { return m_bus; }

private:
    friend class CZCore;
    CZBus(sd_bus *bus, Type type, std::shared_ptr<CZEventSource> eventSource) noexcept;
    sd_bus *m_bus {};
    Type m_type;
    std::shared_ptr<CZEventSource> m_eventSource;
};

#endif // CZBUS_H
