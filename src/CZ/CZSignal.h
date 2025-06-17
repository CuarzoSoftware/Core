#ifndef CZSIGNAL_H
#define CZSIGNAL_H

#include <CZ/CZListener.h>
#include <cassert>

template<class...Args>
class CZ::CZSignal : public CZSignalBase
{
public:

    CZListener *subscribe(CZObject *listenerOwner, const std::function<void(Args...)> &callback) noexcept
    {
        assert("Invalid subscriber object" && listenerOwner);
        listeners.push_back(new CZListenerTemplate<Args...>(listenerOwner, this, callback));
        return listeners.back();
    }

    void notify(Args...data)
    {
        for (auto *listener : listeners)
            setNotified(*listener, false);

    retry:
        changed = false;

        for (auto *listener : listeners)
        {
            if (listener->wasNotified())
                continue;

            setNotified(*listener, true);
            static_cast<CZ::CZListenerTemplate<Args...>*>(listener)->callback()(data...);

            if (changed)
                goto retry;
        }
    }
};

#endif
