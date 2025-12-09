#include <CZ/Core/CZSpringAnimation.h>
#include <cmath>

using namespace CZ;

CZSpringAnimation::CZSpringAnimation(Float64 from, Float64 to, Float64 initialVelocity, Float64 stiffness, Float64 dampingRatio, Callback onFinish, Callback onUpdate) noexcept :
    CZAnimation(onUpdate, onFinish, false),
    v(initialVelocity),
    k(stiffness),
    b(to),
    a(from),
    m_dampingRatio(dampingRatio)
{
    m_value = a;
    m_naturalFreq = std::sqrt(k* 0.01);
}

void CZSpringAnimation::setStiffness(Float64 stiffness) noexcept
{
    k = stiffness;
    m_naturalFreq = std::sqrt(k * 0.01);
}

void CZSpringAnimation::onStart() noexcept
{
    m_value = a;
}

void CZSpringAnimation::onUpdate() noexcept
{
    Float64 dt = 0.001 * std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime()).count();

    computeNextState(dt);

    if (isAtRest())
    {
        m_value = b;
        v = 0;
        m_isRunning = false;
    }
}

bool CZSpringAnimation::isAtRest() const noexcept
{
    const Float64 posThresh = 0.001;
    const Float64 velThresh = 0.001;
    return (std::abs(m_value - b) < posThresh &&
            std::abs(v) < velThresh);
}

void CZSpringAnimation::computeNextState(Float64 dt) noexcept
{
    Float64 delta = m_value - b;

    if (m_dampingRatio < 1)
        computeUnderdamped(delta, dt);
    else if (m_dampingRatio == 1)
        computeCriticallyDamped(delta, dt);
    else
        computeOverdamped(delta, dt);
}

void CZSpringAnimation::computeUnderdamped(Float64 delta, Float64 dt) noexcept
{
    Float64 w_d = m_naturalFreq * std::sqrt(1 - m_dampingRatio * m_dampingRatio);

    Float64 A = delta;
    Float64 B = (v + m_dampingRatio * m_naturalFreq * delta) / w_d;

    Float64 expTerm = std::exp(-m_dampingRatio * m_naturalFreq * dt);

    m_value = b + expTerm * (A * std::cos(w_d * dt) +
                             B * std::sin(w_d * dt));

    v = expTerm * (
            -m_naturalFreq * m_dampingRatio * (A * std::cos(w_d * dt)
                                               + B * std::sin(w_d * dt)) +
            -A * w_d * std::sin(w_d * dt) +
            B * w_d * std::cos(w_d * dt)
            );
}

void CZSpringAnimation::computeCriticallyDamped(Float64 delta, Float64 dt) noexcept
{
    Float64 A = delta;
    Float64 B = v + m_naturalFreq * delta;
    Float64 expTerm = std::exp(-m_naturalFreq * dt);

    m_value = b + expTerm * (A + B * dt);
    v = expTerm * (B - m_naturalFreq * (A + B * dt));
}

void CZSpringAnimation::computeOverdamped(Float64 delta, Float64 dt) noexcept
{
    Float64 d = std::sqrt(m_dampingRatio * m_dampingRatio - 1);
    Float64 r1 = -m_naturalFreq * (m_dampingRatio - d);
    Float64 r2 = -m_naturalFreq * (m_dampingRatio + d);

    Float64 A = (v - r2 * delta) / (r1 - r2);
    Float64 B = delta - A;

    m_value = b + A * std::exp(r1 * dt) + B * std::exp(r2 * dt);
    v = A * r1 * std::exp(r1 * dt) + B * r2 * std::exp(r2 * dt);
}
