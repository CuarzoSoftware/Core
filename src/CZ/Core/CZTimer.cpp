#include <CZ/Core/CZLog.h>
#include <CZ/Core/CZTimer.h>
#include <CZ/Core/CZWeak.h>
#include <CZ/Core/CZCore.h>
#include <CZ/Core/Utils/CZVectorUtils.h>

using namespace CZ;

CZTimer::CZTimer(const Callback &callback) noexcept :
    m_callback(callback),
    m_oneShoot(false)
{
    init();
}

void CZTimer::OneShot(UInt32 timeoutMs, const Callback &callback) noexcept
{
    if (!callback)
        return;

    new CZTimer(true, callback, timeoutMs);
}

CZTimer::~CZTimer() noexcept
{
    auto core { CZCore::Get() };
    if (!core) return;

    core->m_timersChanged = true;
    CZVectorUtils::RemoveOneUnordered(core->m_timers, this);
}

void CZTimer::start(UInt32 timeoutMs) noexcept
{
    auto core { CZCore::Get() };

    if (!core)
    {
        CZLog(CZError, CZLN, "CZTimer started without a CZCore");
        return;
    }

    m_timeoutMs = timeoutMs;

    if (!m_callback)
        return;

    m_running = true;
    m_beginTime = std::chrono::steady_clock::now();
    core->scheduleTimer();
}

void CZTimer::stop(bool notifyIfRunning) noexcept
{
    if (!m_running)
        return;

    m_running = false;

    auto core { CZCore::Get() };

    CZWeak<CZTimer> ref { this };

    if (notifyIfRunning && m_callback)
        m_callback(this);

    if (ref && !m_running && m_oneShoot)
        delete this;

    if (core)
        core->scheduleTimer();
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
    auto core { CZCore::Get() };

    if (!core)
    {
        CZLog(CZError, CZLN, "CZTimer created without a CZCore");
        return;
    }

    core->m_timers.emplace_back(this);
    core->m_timersChanged = true;
}
