#ifndef CZLISTENER_H
#define CZLISTENER_H

#include <CZ/Core/Cuarzo.h>
#include <cstddef>
#include <tuple>
#include <utility>
#include <vector>

class CZ::CZListener
{
public:
    virtual ~CZListener();

    bool wasNotified() const noexcept
    {
        return notified;
    }

    virtual void invoke(void *argsTuple) = 0;

protected:
    CZListener(CZObject *object, CZSignalBase *signal) noexcept;

    friend class CZSignalBase;

    CZSignalBase *signal;
    CZObject *object;
    size_t signalLink{}, objectLink{};
    bool notified{false};
};

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

class CZ::CZSignalBase
{
public:
    ~CZSignalBase() noexcept
    {
        while (!listeners.empty())
            delete listeners.back();
    }
protected:
    CZSignalBase() noexcept = default;
    friend class CZListener;
    std::vector<CZListener*> listeners;
    bool changed { true };
    void setNotified(CZListener &listener, bool state) noexcept
    {
        listener.notified = state;
    }
};

#endif
