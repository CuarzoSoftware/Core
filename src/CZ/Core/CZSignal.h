#ifndef CZSIGNAL_H
#define CZSIGNAL_H

#include <CZ/Core/CZListener.h>
#include <cassert>

template<class...Args>
class CZ::CZSignal : public CZSignalBase
{
public:
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
