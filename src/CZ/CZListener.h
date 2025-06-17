#ifndef CZLISTENER_H
#define CZLISTENER_H

#include <CZ/CZ.h>
#include <cstddef>
#include <functional>

class CZ::CZListener
{
public:
    ~CZListener();

    bool wasNotified() const noexcept
    {
        return notified;
    }
protected:
    CZListener(CZObject *object, CZSignalBase *signal) noexcept;
    friend class CZSignalBase;
    CZSignalBase *signal;
    CZObject *object;
    size_t signalLink, objectLink;
    bool notified { false };
};

template<class...Args>
class CZ::CZListenerTemplate : public CZListener
{
public:
    CZListenerTemplate(CZObject *object, CZSignalBase *signal, const std::function<void(Args...)> &callback) noexcept :
        CZListener(object, signal),
        m_callback(callback)
    {}

    const std::function<void(Args...)> &callback() const noexcept
    {
        return m_callback;
    }
private:
    std::function<void(Args...)> m_callback;
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
