#include <CZ/Core/CZCore.h>
#include <CZ/Core/CZAnimation.h>
#include <CZ/Core/CZTimer.h>
#include <CZ/Core/CZLog.h>
#include <algorithm>

using namespace CZ;

CZAnimation::CZAnimation(Callback onUpdate, Callback onFinish, bool oneshot) noexcept :
    m_onUpdate(onUpdate),
    m_onFinish(onFinish),
    m_oneshot(oneshot)
{
    auto core { CZCore::Get() };
    assert(core && "CZAnimations must be created after a CZCore");
    core->m_animations.push_back(this);
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

    auto it { std::find(core->m_animations.begin(), core->m_animations.end(), this) };
    if (it != core->m_animations.end())
    {
        core->m_animationsChanged = true;
        core->m_animations.erase(it);
    }
}

void CZAnimation::setOnUpdateCallback(Callback onUpdate) noexcept
{
    if (isRunning())
        return;

    m_onUpdate = onUpdate;
}

void CZAnimation::setOnFinishCallback(Callback onFinish) noexcept
{
    if (isRunning())
        return;

    m_onFinish = onFinish;
}

void CZAnimation::start() noexcept
{
    if (isRunning())
        return;

    m_startTime = std::chrono::steady_clock::now();
    m_isRunning = true;
    onStart();

    auto core { CZCore::Get() };

    if (!core)
    {
        CZLog(CZWarning, CZLN, "CZCore destroyed before CZAnimation");
        return;
    }

    core->m_animationsTimer->start(8);

    if (m_onUpdate)
        m_onUpdate(this);
}

void CZAnimation::stop() noexcept
{
    m_isRunning = false;
    m_pendingDestroy = m_oneshot;
}
