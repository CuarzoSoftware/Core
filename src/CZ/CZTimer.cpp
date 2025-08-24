#include <CZ/CZTimer.h>
#include <CZ/CZWeak.h>

using namespace CZ;

CZTimer::CZTimer(const Callback &callback) noexcept :
    m_callback(callback),
    m_oneShoot(false)
{
    init();
}

void CZTimer::OneShot(UInt64 timeoutMs, const Callback &callback) noexcept
{
    if (!callback)
        return;

    new CZTimer(true, callback, timeoutMs);
}

void CZTimer::start(UInt64 timeoutMs) noexcept
{
    m_timeoutMs = timeoutMs;

    if (!m_callback)
        return;

    m_running = true;

    if (timeoutMs == 0)
    {
        constexpr itimerspec timeout { {0, 0}, {0, 1} };
        timerfd_settime(m_source->fd(), 0, &timeout, nullptr);
    }
    else
    {
        const itimerspec timeout ( {0, 0}, {static_cast<__time_t>(timeoutMs / 1000), static_cast<__syscall_slong_t>((timeoutMs % 1000) * 1000000)} );
        timerfd_settime(m_source->fd(), 0, &timeout, nullptr);
    }
}

void CZTimer::stop(bool notifyIfRunning) noexcept
{
    if (!m_running)
        return;

    m_running = false;
    constexpr itimerspec timeout { {0, 0}, {0, 0} };
    timerfd_settime(m_source->fd(), 0, &timeout, nullptr);

    CZWeak<CZTimer> ref { this };

    if (notifyIfRunning && m_callback)
        m_callback(this);

    if (ref && !m_running && m_oneShoot)
        delete this;
}

CZTimer::CZTimer(bool oneShoot, const Callback &callback, UInt64 timeoutMs) noexcept :
    m_callback(callback),
    m_oneShoot(oneShoot)
{
    init();
    start(timeoutMs);
}

void CZTimer::init() noexcept
{
    m_source = CZEventSource::Make(timerfd_create(CLOCK_MONOTONIC, TFD_CLOEXEC), EPOLLIN, [this](int fd, UInt32) {
        UInt64 expirations;
        assert(read(fd, &expirations, sizeof(expirations)) == sizeof(expirations));

        constexpr itimerspec timeout { {0, 0}, {0, 0} };
        timerfd_settime(fd, 0, &timeout, nullptr);

        const bool wasRunning { m_running };
        CZWeak<CZTimer> ref { this };
        m_running = false;

        if (wasRunning && m_callback)
            m_callback(this);

        if (ref && !m_running && m_oneShoot)
            delete this;
    });

    assert("CZTimers must be created after a CZCore" && m_source);
}
