#ifndef CZBUS_H
#define CZBUS_H

#include <CZ/Core/CZObject.h>
#include <memory>

struct sd_bus;

/**
 * @brief Shared sd-bus (D-Bus) connection integrated into the CZCore event loop.
 *
 * There is at most one system and one user bus connection per process. They are created on demand
 * by GetOrMakeSystem() / GetOrMakeUser() and kept alive only while someone holds the returned
 * `std::shared_ptr`. Incoming messages are dispatched from the event loop through eventSource(), and
 * CZCore takes timeoutMS() into account when waiting for events.
 *
 * Use bus() with the sd-bus API to make calls, add matches, etc.
 */
class CZ::CZBus : public CZObject
{
public:
    /** @brief Bus kind. */
    enum class Type
    {
        System, ///< The system bus.
        User    ///< The user (session) bus.
    };

    ~CZBus() noexcept;

    /** @brief Returns the system bus connection, connecting if none is alive. Returns `nullptr` on failure. */
    [[nodiscard]] static std::shared_ptr<CZBus> GetOrMakeSystem() noexcept;

    /** @brief Returns the system bus connection if one is alive, otherwise `nullptr`. */
    static std::shared_ptr<CZBus> GetSystem() noexcept;

    /** @brief Returns the user bus connection, connecting if none is alive. Returns `nullptr` on failure. */
    [[nodiscard]] static std::shared_ptr<CZBus> GetOrMakeUser() noexcept;

    /** @brief Returns the user bus connection if one is alive, otherwise `nullptr`. */
    static std::shared_ptr<CZBus> GetUser() noexcept;

    /** @brief Event source that dispatches incoming messages from the event loop. */
    std::shared_ptr<CZEventSource> eventSource() const noexcept { return m_eventSource; }

    /** @brief Time in milliseconds until sd-bus needs processing (rounded up), or -1 if there is no timeout or on error. */
    int timeoutMS() const noexcept;

    /** @brief The underlying sd-bus connection. */
    sd_bus *bus() const noexcept { return m_bus; }

    /**
     * @brief Processes messages sd-bus already read but didn't dispatch.
     *
     * A blocking call (e.g. sd_bus_call_method()) reads every message that arrives while it waits
     * into an internal queue. The socket is then empty, so the event source isn't woken for them.
     * CZCore calls this after each dispatch.
     */
    void processQueued() noexcept;

private:
    friend class CZCore;
    CZBus(sd_bus *bus, Type type, std::shared_ptr<CZEventSource> eventSource) noexcept;
    sd_bus *m_bus {};
    Type m_type;
    std::shared_ptr<CZEventSource> m_eventSource;
};

#endif // CZBUS_H
