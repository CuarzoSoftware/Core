#ifndef CZSIGNAL_H
#define CZSIGNAL_H

#include <CZ/Core/CZListener.h>
#include <cassert>

/**
 * @brief Notifies subscribed callbacks with arguments of types @p Args.
 *
 * Callbacks are subscribed with subscribe(), each owned by a CZObject: when the owner is destroyed,
 * its subscriptions are removed automatically, so callbacks never run on a destroyed owner.
 *
 * @code
 * CZSignal<int> onValueChanged;
 *
 * onValueChanged.subscribe(this, [](int value) {
 *     // ...
 * });
 *
 * onValueChanged.notify(5);
 * @endcode
 *
 * Listeners can be subscribed or destroyed (and owners deleted) from within a callback while the
 * signal is being emitted. Listeners subscribed during an emission don't receive it.
 */
template<class...Args>
class CZ::CZSignal : public CZSignalBase
{
public:
    /**
     * @brief Subscribes a callback.
     *
     * @param listenerOwner Object that owns the subscription (must not be `nullptr`). The listener is
     *                      destroyed when the owner or the signal is destroyed.
     * @param callback Callable invoked with the signal arguments.
     * @return The listener. Delete it to unsubscribe earlier.
     */
    template<typename F>
    CZListener* subscribe(CZObject *listenerOwner, F&& callback) noexcept
    {
        assert(listenerOwner);

        using Fn = std::decay_t<F>;

        listeners.push_back(
            new CZListenerTemplate<Fn, Args...>(
                listenerOwner,
                this,
                std::forward<F>(callback)
                )
            );

        return listeners.back();
    }

    /** @brief Invokes each subscribed callback once with @p data. */
    void notify(Args... data)
    {
        std::tuple<Args...> argsTuple(std::forward<Args>(data)...);

        for (auto *listener : listeners)
            setNotified(*listener, false);

    retry:
        changed = false;

        for (auto *listener : listeners)
        {
            if (listener->wasNotified())
                continue;

            setNotified(*listener, true);
            listener->invoke(&argsTuple);

            if (changed)
                goto retry;
        }
    }
};

#endif
