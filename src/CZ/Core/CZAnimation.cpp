#include <CZ/Core/CZCore.h>
#include <CZ/Core/CZAnimation.h>
#include <CZ/Core/CZTimer.h>
#include <CZ/Core/CZLog.h>
#include <algorithm>

using namespace CZ;

CZAnimation::CZAnimation(Callback onUpdateCb, Callback onFinishCb, bool oneshot) noexcept :
    m_oneshot(oneshot)
{
    auto core { CZCore::Get() };
    assert(core && "CZAnimations must be created after a CZCore");
    {
        std::lock_guard<std::recursive_mutex> lock(core->m_animationsMutex);
        core->m_animations.push_back(this);
        /* A callback creating an animation invalidates a running
         * updateAnimations() pass just like a destruction does:
         * push_back can reallocate the vector under the range-for,
         * and the pass retries only on m_animationsChanged — which
         * until now only the destructor raised. */
        core->m_animationsChanged = true;
    }

    // Constructor callbacks are kept as a convenience: subscribe them to the signals, owned by the
    // animation itself so they live exactly as long as it does.
    if (onUpdateCb)
        onUpdate.subscribe(this, std::move(onUpdateCb));

    if (onFinishCb)
        onFinish.subscribe(this, std::move(onFinishCb));
}

CZAnimation::~CZAnimation() noexcept
{
    notifyDestruction();

    auto core { CZCore::Get() };

    if (!core)
    {
        CZLog(CZWarning, CZLN, "CZCore destroyed before CZAnimation");
        return;
    }

    std::lock_guard<std::recursive_mutex> lock(core->m_animationsMutex);
    auto it { std::find(core->m_animations.begin(), core->m_animations.end(), this) };
    if (it != core->m_animations.end())
    {
        core->m_animationsChanged = true;
        core->m_animations.erase(it);
    }
}

void CZAnimation::start() noexcept
{
    if (isRunning())
        return;

    reset();
}

void CZAnimation::reset() noexcept
{
    m_startTime = std::chrono::steady_clock::now();
    m_isRunning = true;
    onStart();

    auto core { CZCore::Get() };

    if (!core)
    {
        CZLog(CZWarning, CZLN, "CZCore destroyed before CZAnimation");
        return;
    }

    /* Respect the disabled interval: a compositor that took over
     * animation pacing (setAnimationInterval(0)) must not get a
     * stray 8ms tick per start. */
    if (core->m_animationInteval > 0)
        core->m_animationsTimer->start(8);

    onUpdate.notify(this);
}

void CZAnimation::pause() noexcept
{
    m_isRunning = false;
    m_pendingDestroy = m_oneshot;
}
