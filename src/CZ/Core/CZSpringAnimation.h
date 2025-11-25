#ifndef CZ_CZSPRINGANIMATION_H
#define CZ_CZSPRINGANIMATION_H

#include <CZ/Core/CZAnimation.h>

/**
 * @brief Implements a physics-based spring animation using a damping harmonic oscillator model.
 *
 * This class simulates a spring system (mass-spring-damper) to provide natural, fluid
 * animation movement. The behavior is controlled by two primary parameters:
 * **stiffness** (the strength of the spring force) and **damping ratio** (the level of
 * resistance/friction, controlling bounciness).
 *
 * @note Unlike time-based animations, spring animations are not duration-dependent.
 * They run until the kinetic energy of the system drops below a minimal threshold.
 * The resulting duration and speed depend on the spring constants and the distance
 * being animated.
 */
class CZ::CZSpringAnimation : public CZAnimation
{
public:
    /**
     * @brief Extremely high stiffness, resulting in a very strong, fast restoring force.
     * Use for animations that should settle nearly instantly.
     */
    static constexpr Float64 StiffnessHigh { 10000.0 };

    /**
     * @brief Medium stiffness, often suitable as a default for mid-range animations.
     */
    static constexpr Float64 StiffnessMedium { 1500.0 };

    /**
     * @brief Low stiffness, resulting in a weaker, slower restoring force.
     * Good for subtle or gradual movements over large ranges, or standard movement over small ranges.
     */
    static constexpr Float64 StiffnessLow { 200.0 };

    /**
     * @brief Very low stiffness, resulting in a gentle, slow animation.
     * Ideal for animating very small changes (e.g., from 0 to 1) or for extremely subtle movements.
     */
    static constexpr Float64 StiffnessVeryLow { 50.0 };

    /**
     * @brief High bounciness (0.2 < ratio < 1.0). Significant oscillation and overshoot.
     * Use for playful, pronounced, or micro-interaction effects.
     */
    static constexpr Float64 DampingRatioHighBouncy { 0.2 };

    /**
     * @brief Medium bounciness (0.5 < ratio < 1.0). Moderate oscillation and overshoot.
     * This is a common choice for general UI movement.
     */
    static constexpr Float64 DampingRatioMediumBouncy { 0.5 };

    /**
     * @brief Low bounciness (0.75 < ratio < 1.0). Subtle oscillation and minimal overshoot.
     */
    static constexpr Float64 DampingRatioLowBouncy { 0.75 };

    /**
     * @brief Critically damped (ratio = 1.0). The animation settles fastest without any overshoot.
     * Ideal for quick, non-bouncy snaps or stopping motions smoothly.
     */
    static constexpr Float64 DampingRatioNoBouncy { 1.0 };

    /**
     * @brief Constructs a CZSpringAnimation object.
     *
     * @param from The starting value of the animation.
     * @param to The target (final) value of the animation.
     * @param initialVelocity The starting velocity of the system. Positive if moving towards the 'to' value.
     * @param stiffness The spring constant. Controls the strength of the restoring force.
     * @param dampingRatio The ratio. Controls the decay of oscillations (bounciness).
     * @param onUpdate Callback executed on every animation step with the current value.
     * @param onFinish Callback executed when the animation comes to rest.
     */
    CZSpringAnimation(Float64 from = 0.0,
                      Float64 to = 1.0,
                      Float64 initialVelocity = 0.0,
                      Float64 stiffness = StiffnessLow,
                      Float64 dampingRatio = DampingRatioLowBouncy,
                      Callback onUpdate = nullptr, Callback onFinish = nullptr) noexcept;

    /**
     * @brief The current velocity.
     */
    Float64 velocity() const noexcept { return v; }

    /**
     * @brief The current starting value.
     */
    Float64 from() const noexcept { return a; }

    /**
     * @brief The current target value.
     */
    Float64 to() const noexcept { return b; }

    /**
     * @brief The current damping ratio.
     */
    Float64 dampingRation() const noexcept { return m_dampingRatio; }

    /**
     * @brief he current stiffness.
     */
    Float64 stiffness() const noexcept { return k; }

    /**
     * @brief Sets a new target value for the animation.
     *
     * This can be called while the animation is running to smoothly interrupt
     * the current movement and redirect it to a new destination. This is the
     * suggested way to change the animation's goal mid-flight.
     *
     * @param value The new target 'to' value.
     */
    void setTo(Float64 value) noexcept { b = value; }

    /**
     * @brief Sets the animation's starting value.
     * @param value The new starting 'from' value.
     */
    void setFrom(Float64 value) noexcept { a = value; }

    /**
     * @brief Directly sets the current animated value.
     *
     * This can be used to manually place the animated object at a specific point
     * while the animation is running.
     *
     * @param value The new current value.
     */
    void setValue(Float64 value) noexcept { m_value = value; }

    /**
     * @brief Sets the damping ratio for the spring.
     * @param ratio The new damping ratio.
     */
    void setDampingRatio(Float64 ratio) noexcept { m_dampingRatio = ratio; }

    /**
     * @brief Sets the stiffness for the spring.
     *
     * @param stiffness The new stiffness.
     */
    void setStiffness(Float64 stiffness) noexcept;

    /**
     * @brief Sets the current velocity of the spring system.
     *
     * Setting the velocity can be used to initiate an animation with a 'flick'
     * or to alter the momentum of a currently running animation.
     *
     * @param vel The new velocity.
     */
    void setVelocity(Float64 vel) noexcept { v = vel; }

protected:
    void onStart() noexcept override;
    void onUpdate() noexcept override;

private:
    Float64 v, k, b, a;
    Float64 m_naturalFreq, m_dampingRatio;

    bool isAtRest() const noexcept;
    void computeNextState(Float64 dt) noexcept;
    void computeUnderdamped(Float64 delta, Float64 dt) noexcept;
    void computeCriticallyDamped(Float64 delta, Float64 dt) noexcept;
    void computeOverdamped(Float64 delta, Float64 dt) noexcept;
};

#endif // CZ_CZSPRINGANIMATION_H
