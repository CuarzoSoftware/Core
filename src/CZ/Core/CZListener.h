#ifndef CZLISTENER_H
#define CZLISTENER_H

#include <CZ/Core/Cuarzo.h>
#include <cstddef>
#include <tuple>
#include <utility>
#include <vector>

/**
 * @brief A subscription of a callback to a CZSignal.
 *
 * Created by CZSignal::subscribe(). A listener is owned by both its signal and its owner object:
 * it is destroyed automatically when either of them is destroyed. Deleting it earlier
 * unsubscribes the callback. It is safe to do so (or to subscribe new listeners) while the signal
 * is being emitted.
 */
class CZ::CZListener
{
public:
    /** @brief Unsubscribes the callback, removing the listener from its signal and owner object. */
    virtual ~CZListener();

    /**
     * @brief Whether the callback was already invoked during the current (or last) emission.
     *
     * Listeners subscribed during an emission are marked as notified so they don't receive it.
     */
    bool wasNotified() const noexcept
    {
        return notified;
    }

    /** @brief Invokes the callback with a pointer to a `std::tuple` of the signal arguments. Used by CZSignal::notify(). */
    virtual void invoke(void *argsTuple) = 0;

protected:
    /** @brief Registers the listener in @p object and @p signal. */
    CZListener(CZObject *object, CZSignalBase *signal) noexcept;

    friend class CZSignalBase;

    /// @cond INTERNAL
    CZSignalBase *signal;
    CZObject *object;
    size_t signalLink{}, objectLink{};
    bool notified{false};
    /// @endcond
};

/// @cond INTERNAL
template<class F, class...Args>
class CZ::CZListenerTemplate final : public CZListener
{
public:
    CZListenerTemplate(CZObject *object, CZSignalBase *signal, F&& callback) noexcept
        : CZListener(object, signal)
        , m_callback(std::forward<F>(callback))
    {}

    void invoke(void *argsTuple) override
    {
        auto &t = *static_cast<std::tuple<Args...>*>(argsTuple);
        call(t, std::index_sequence_for<Args...>{});
    }

private:
    template<std::size_t... I>
    void call(std::tuple<Args...> &t, std::index_sequence<I...>)
    {
        m_callback(std::get<I>(t)...);
    }

    F m_callback;
};
/// @endcond

/**
 * @brief Type-erased base class of CZSignal.
 *
 * Owns the subscribed CZListener objects and destroys them when the signal is destroyed.
 */
class CZ::CZSignalBase
{
public:
    /** @brief Destroys all subscribed listeners. */
    ~CZSignalBase() noexcept
    {
        while (!listeners.empty())
            delete listeners.back();
    }
protected:
    /// @cond INTERNAL
    CZSignalBase() noexcept = default;
    friend class CZListener;
    std::vector<CZListener*> listeners;
    bool changed { true };
    void setNotified(CZListener &listener, bool state) noexcept
    {
        listener.notified = state;
    }
    /// @endcond
};

#endif
