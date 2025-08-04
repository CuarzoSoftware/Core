#ifndef CZ_CZBOOLEANEVENTSOURCE_H
#define CZ_CZBOOLEANEVENTSOURCE_H

#include <CZ/CZEventSource.h>
#include <memory>

/**
 * @brief A toggleable event source.
 *
 * When enabled, this event source wakes up the main loop. Once processed,
 * it disables itself and the provided callback is triggered. The event source can be re-enabled
 * during the callback or at any later time using `setState()`.
 *
 * Internally, it uses an `eventfd`.
 */
class CZ::CZBooleanEventSource final : public CZObject
{
public:

    /**
     * @brief Callback function type. Called when the event is triggered.
     */
    using Callback = std::function<void(CZBooleanEventSource *source)>;

    /**
     * @brief Creates an AKBooleanEventSource.
     *
     * @param callback The callback function to be triggered (optional).
     * @param enabled Initial state of the event source.
     * @return A shared pointer to the created AKBooleanEventSource, or `nullptr` on failure.
     */
    static std::shared_ptr<CZBooleanEventSource> Make(bool enabled, const Callback &callback) noexcept;

    /**
     * @brief Gets the current state of the event source.
     */
    bool state() const noexcept { return m_state; };

    /**
     * @brief Sets the state of the event source.
     */
    void setState(bool enabled) noexcept;

    /**
     * @brief Destructor for AKBooleanEventSource.
     */
    ~CZBooleanEventSource() noexcept;

private:
    CZBooleanEventSource(bool enabled, const Callback &callback) noexcept :
        m_callback(callback), m_state(enabled) {}
    std::shared_ptr<CZEventSource> m_source;
    Callback m_callback { nullptr };
    bool m_state;
};

#endif // CZ_CZBOOLEANEVENTSOURCE_H
