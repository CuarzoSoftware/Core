#include <CZ/Core/CZLinearAnimation.h>

using namespace CZ;

CZLinearAnimation::CZLinearAnimation(UInt32 durationMs, Callback onUpdate, Callback onFinish) noexcept :
    CZAnimation(onUpdate, onFinish, false),
    m_duration(durationMs)
{}

CZLinearAnimation::CZLinearAnimation(bool oneshot, UInt32 durationMs, Callback onUpdate, Callback onFinish) noexcept :
    CZAnimation(onUpdate, onFinish, oneshot),
    m_duration(durationMs)
{}

void CZLinearAnimation::OneShot(UInt32 durationMs, Callback onUpdate, Callback onFinish) noexcept
{
    CZLinearAnimation *anim { new CZLinearAnimation(true, durationMs, onUpdate, onFinish) };
    anim->start();
}

void CZLinearAnimation::onStart() noexcept
{
    m_value = 0;
}

void CZLinearAnimation::onUpdate() noexcept
{
    Int64 elapsed  { std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime()).count() };
    Int64 duration { static_cast<Int64>(m_duration) };

    if (elapsed >= duration)
    {
        m_isRunning = false;
        m_value = 1.0;
    }
    else
        m_value = static_cast<Float64>(elapsed)/static_cast<Float64>(duration);
}
