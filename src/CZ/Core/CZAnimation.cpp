#include <CZ/Core/CZCore.h>
#include <CZ/Core/CZAnimation.h>
#include <CZ/Core/CZTimer.h>
#include <CZ/Core/CZLog.h>
#include <algorithm>

using namespace CZ;

CZAnimation::CZAnimation(UInt32 durationMs, const Callback &onUpdate, const Callback &onFinish) noexcept :
    m_onUpdate(onUpdate),
    m_duration(durationMs),
    m_onFinish(onFinish)
{
    auto core { CZCore::Get() };
    assert(core && "CZAnimations must be created after a CZCore");
    core->m_animations.push_back(this);
}

CZAnimation::~CZAnimation() noexcept
{
    notifyDestruction();
    stop();

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

void CZAnimation::OneShot(UInt32 durationMs, const Callback &onUpdate, const Callback &onFinish) noexcept
{
    CZAnimation *anim { new CZAnimation(durationMs, onUpdate, onFinish) };
    anim->m_destroyOnFinish = true;
    anim->start();
}

void CZAnimation::setOnUpdateCallback(const Callback &onUpdate) noexcept
{
    if (m_running)
        return;

    m_onUpdate = onUpdate;
}

void CZAnimation::setOnFinishCallback(const Callback &onFinish) noexcept
{
    if (m_running)
        return;

    m_onFinish = onFinish;
}

void CZAnimation::start() noexcept
{
    if (m_running)
        return;

    m_value = 0.0;
    m_beginTime = std::chrono::steady_clock::now();
    m_running = true;

    auto core { CZCore::Get() };

    if (!core)
    {
        CZLog(CZWarning, CZLN, "CZCore destroyed before CZAnimation");
        return;
    }

    if (m_onUpdate)
    {
        core->m_animationsTimer->start(8);
        m_onUpdate(this);
    }
}

void CZAnimation::stop()
{
    if (!m_running)
        return;

    m_value = 1.0;
    m_running = false;

    if (m_onFinish)
        m_onFinish(this);

    if (m_destroyOnFinish)
        m_pendingDestroy = true;
}
