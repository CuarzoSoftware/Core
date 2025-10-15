#ifndef CZ_CZEASE_H
#define CZ_CZEASE_H

#include <CZ/Core/Cuarzo.h>
#include <cmath>

/**
 * @brief Easing function utilities.
 *
 * Each function takes a linear interpolation value ranging from 0 to 1 and
 * outputs a processed value in the same range.
 */
class CZ::CZEase
{
public:

    // ===================================================================================
    // Quadratic Easing
    // ===================================================================================

    /**
     * @brief Quadratic ease-in function.
     *
     * Starts motion from zero velocity and accelerates with a quadratic curve.
     *
     * @param t Normalized time in the range [0.0, 1.0].
     * @return Eased value in the same range.
     */
    static constexpr Float64 InQuad(Float64 t) noexcept { return t * t; }

    /**
     * @brief Quadratic ease-out function.
     *
     * Ends motion by decelerating to zero velocity, following a quadratic curve.
     *
     * @param t Normalized time in the range [0.0, 1.0].
     * @return Eased value in the same range.
     */
    static constexpr Float64 OutQuad(Float64 t) noexcept { return -t * (t - 2.0); }

    /**
     * @brief Quadratic ease-in-out function.
     *
     * Accelerates until halfway, then decelerates, following a quadratic curve.
     *
     * @param t Normalized time in the range [0.0, 1.0].
     * @return Eased value in the same range.
     */
    static constexpr Float64 InOutQuad(Float64 t) noexcept
    {
        t *= 2.0;
        if (t < 1.0)
            return 0.5 * t * t;
        t -= 1.0;
        return -0.5 * (t * (t - 2.0) - 1.0);
    }


    // ===================================================================================
    // Cubic Easing
    // ===================================================================================

    /**
     * @brief Cubic ease-in function.
     *
     * Accelerates from zero velocity following a cubic curve.
     *
     * @param t Normalized time in the range [0.0, 1.0].
     * @return Eased value in the same range.
     */
    static constexpr Float64 InCubic(Float64 t) noexcept { return t * t * t; }

    /**
     * @brief Cubic ease-out function.
     *
     * Decelerates to zero velocity following a cubic curve.
     *
     * @param t Normalized time in the range [0.0, 1.0].
     * @return Eased value in the same range.
     */
    static constexpr Float64 OutCubic(Float64 t) noexcept
    {
        t -= 1.0;
        return t * t * t + 1.0;
    }

    /**
     * @brief Cubic ease-in-out function.
     *
     * Accelerates until halfway, then decelerates, following a cubic curve.
     *
     * @param t Normalized time in the range [0.0, 1.0].
     * @return Eased value in the same range.
     */
    static constexpr Float64 InOutCubic(Float64 t) noexcept
    {
        t *= 2.0;
        if (t < 1.0)
            return 0.5 * t * t * t;
        t -= 2.0;
        return 0.5 * (t * t * t + 2.0);
    }


    // ===================================================================================
    // Quartic Easing
    // ===================================================================================

    /**
     * @brief Quartic ease-in function.
     *
     * Accelerates from zero velocity following a quartic curve.
     *
     * @param t Normalized time in the range [0.0, 1.0].
     * @return Eased value in the same range.
     */
    static constexpr Float64 InQuart(Float64 t) noexcept { return t * t * t * t; }

    /**
     * @brief Quartic ease-out function.
     *
     * Decelerates to zero velocity following a quartic curve.
     *
     * @param t Normalized time in the range [0.0, 1.0].
     * @return Eased value in the same range.
     */
    static constexpr Float64 OutQuart(Float64 t) noexcept
    {
        t -= 1.0;
        return -(t * t * t * t - 1.0);
    }

    /**
     * @brief Quartic ease-in-out function.
     *
     * Accelerates until halfway, then decelerates following a quartic curve.
     *
     * @param t Normalized time in the range [0.0, 1.0].
     * @return Eased value in the same range.
     */
    static constexpr Float64 InOutQuart(Float64 t) noexcept
    {
        t *= 2.0;
        if (t < 1.0)
            return 0.5 * t * t * t * t;
        t -= 2.0;
        return -0.5 * (t * t * t * t - 2.0);
    }


    // ===================================================================================
    // Quintic Easing
    // ===================================================================================

    /**
     * @brief Quintic ease-in function.
     *
     * Accelerates from zero velocity following a quintic curve.
     *
     * @param t Normalized time in the range [0.0, 1.0].
     * @return Eased value in the same range.
     */
    static constexpr Float64 InQuint(Float64 t) noexcept { return t * t * t * t * t; }

    /**
     * @brief Quintic ease-out function.
     *
     * Decelerates to zero velocity following a quintic curve.
     *
     * @param t Normalized time in the range [0.0, 1.0].
     * @return Eased value in the same range.
     */
    static constexpr Float64 OutQuint(Float64 t) noexcept
    {
        t -= 1.0;
        return t * t * t * t * t + 1.0;
    }

    /**
     * @brief Quintic ease-in-out function.
     *
     * Accelerates until halfway, then decelerates following a quintic curve.
     *
     * @param t Normalized time in the range [0.0, 1.0].
     * @return Eased value in the same range.
     */
    static constexpr Float64 InOutQuint(Float64 t) noexcept
    {
        t *= 2.0;
        if (t < 1.0)
            return 0.5 * t * t * t * t * t;
        t -= 2.0;
        return 0.5 * (t * t * t * t * t + 2.0);
    }


    // ===================================================================================
    // Back Easing
    // ===================================================================================

    /**
     * @brief Back ease-in function.
     *
     * Accelerates with an overshoot effect, going slightly backward before moving forward.
     *
     * @param t Normalized time in the range [0.0, 1.0].
     * @param s Overshoot amount controlling how far the animation goes beyond the start.
     *          Default is 1.70158 for a standard overshoot.
     * @return Eased value in the same range.
     */
    static constexpr Float64 InBack(Float64 t, Float64 s = 1.70158) noexcept { return t * t * ((s + 1.0) * t - s); }

    /**
     * @brief Back ease-out function.
     *
     * Decelerates with an overshoot effect, going beyond the target then settling back.
     *
     * @param t Normalized time in the range [0.0, 1.0].
     * @param s Overshoot amount controlling how far the animation goes beyond the end.
     *          Default is 1.70158 for a standard overshoot.
     * @return Eased value in the same range.
     */
    static constexpr Float64 OutBack(Float64 t, Float64 s = 1.70158) noexcept
    {
        t -= 1.0;
        return t * t * ((s + 1.0) * t + s) + 1.0;
    }

    /**
     * @brief Back ease-in-out function.
     *
     * Accelerates and decelerates with overshoot, creating a bounce effect at start and end.
     *
     * @param t Normalized time in the range [0.0, 1.0].
     * @param s Overshoot amount controlling how far the animation overshoots.
     *          Default is 1.70158, scaled internally for ease-in-out behavior.
     * @return Eased value in the same range.
     */
    static constexpr Float64 InOutBack(Float64 t, Float64 s = 1.70158) noexcept
    {
        s *= 1.525;
        t *= 2.0;
        if (t < 1.0)
            return 0.5 * (t * t * ((s + 1.0) * t - s));
        t -= 2.0;
        return 0.5 * (t * t * ((s + 1.0) * t + s) + 2.0);
    }


    // ===================================================================================
    // Bounce Easing
    // ===================================================================================

    /**
     * @brief Bounce ease-out function.
     *
     * Decelerates with multiple bounces, simulating a ball dropping and bouncing to rest.
     *
     * @param t Normalized time in the range [0.0, 1.0].
     * @return Eased value in the same range.
     */
    static constexpr Float64 OutBounce(Float64 t) noexcept
    {
        if (t < 1.0 / 2.75)
            return 7.5625 * t * t;
        else if (t < 2.0 / 2.75)
        {
            t -= 1.5 / 2.75;
            return 7.5625 * t * t + 0.75;
        }
        else if (t < 2.5 / 2.75)
        {
            t -= 2.25 / 2.75;
            return 7.5625 * t * t + 0.9375;
        }
        else
        {
            t -= 2.625 / 2.75;
            return 7.5625 * t * t + 0.984375;
        }
    }

    /**
     * @brief Bounce ease-in function.
     *
     * Accelerates with multiple bounces, mirroring the bounce ease-out in reverse.
     *
     * @param t Normalized time in the range [0.0, 1.0].
     * @return Eased value in the same range.
     */
    static constexpr Float64 InBounce(Float64 t) noexcept { return 1.0 - OutBounce(1.0 - t); }

    /**
     * @brief Bounce ease-in-out function.
     *
     * Accelerates with bounces in the first half, then decelerates with bounces in the second half.
     *
     * @param t Normalized time in the range [0.0, 1.0].
     * @return Eased value in the same range.
     */
    static constexpr Float64 InOutBounce(Float64 t) noexcept
    {
        if (t < 0.5)
            return InBounce(t * 2.0) * 0.5;
        return OutBounce(t * 2.0 - 1.0) * 0.5 + 0.5;
    }


    // ===================================================================================
    // Sine Easing
    // ===================================================================================

    /**
     * @brief Sine ease-in function.
     *
     * Accelerates smoothly from zero velocity following a sine curve.
     *
     * @param t Normalized time in the range [0.0, 1.0].
     * @return Eased value in the same range.
     */
    static constexpr Float64 InSine(Float64 t) noexcept { return 1.0 - std::cos(t * M_PI / 2.0); }

    /**
     * @brief Sine ease-out function.
     *
     * Decelerates smoothly to zero velocity following a sine curve.
     *
     * @param t Normalized time in the range [0.0, 1.0].
     * @return Eased value in the same range.
     */
    static constexpr Float64 OutSine(Float64 t) noexcept { return std::sin(t * M_PI / 2.0); }

    /**
     * @brief Sine ease-in-out function.
     *
     * Accelerates until halfway, then decelerates smoothly following a sine curve.
     *
     * @param t Normalized time in the range [0.0, 1.0].
     * @return Eased value in the same range.
     */
    static constexpr Float64 InOutSine(Float64 t) noexcept { return -0.5 * (std::cos(M_PI * t) - 1.0); }


    // ===================================================================================
    // Exponential Easing
    // ===================================================================================

    /**
     * @brief Exponential ease-in function.
     *
     * Accelerates rapidly from zero velocity with exponential growth.
     *
     * @param t Normalized time in the range [0.0, 1.0].
     * @return Eased value in the same range.
     */
    static constexpr Float64 InExpo(Float64 t) noexcept
    {
        if (t <= 0.0)
            return 0.0;
        return std::pow(2.0, 10.0 * (t - 1.0));
    }

    /**
     * @brief Exponential ease-out function.
     *
     * Decelerates rapidly to zero velocity with exponential decay.
     *
     * @param t Normalized time in the range [0.0, 1.0].
     * @return Eased value in the same range.
     */
    static constexpr Float64 OutExpo(Float64 t) noexcept
    {
        if (t >= 1.0)
            return 1.0;
        return 1.0 - std::pow(2.0, -10.0 * t);
    }

    /**
     * @brief Exponential ease-in-out function.
     *
     * Accelerates exponentially until halfway, then decelerates exponentially.
     *
     * @param t Normalized time in the range [0.0, 1.0].
     * @return Eased value in the same range.
     */
    static constexpr Float64 InOutExpo(Float64 t) noexcept
    {
        if (t <= 0.0)
            return 0.0;
        if (t >= 1.0)
            return 1.0;
        t *= 2.0;
        if (t < 1.0)
            return 0.5 * std::pow(2.0, 10.0 * (t - 1.0));
        return 0.5 * (2.0 - std::pow(2.0, -10.0 * (t - 1.0)));
    }


    // ===================================================================================
    // Circular Easing
    // ===================================================================================

    /**
     * @brief Circular ease-in function.
     *
     * Accelerates from zero velocity following a circular curve.
     *
     * @param t Normalized time in the range [0.0, 1.0].
     * @return Eased value in the same range.
     */
    static constexpr Float64 InCirc(Float64 t) noexcept { return -(std::sqrt(1.0 - t * t) - 1.0); }

    /**
     * @brief Circular ease-out function.
     *
     * Decelerates to zero velocity following a circular curve.
     *
     * @param t Normalized time in the range [0.0, 1.0].
     * @return Eased value in the same range.
     */
    static constexpr Float64 OutCirc(Float64 t) noexcept
    {
        t -= 1.0;
        return std::sqrt(1.0 - t * t);
    }

    /**
     * @brief Circular ease-in-out function.
     *
     * Accelerates following a circular curve until halfway, then decelerates.
     *
     * @param t Normalized time in the range [0.0, 1.0].
     * @return Eased value in the same range.
     */
    static constexpr Float64 InOutCirc(Float64 t) noexcept
    {
        t *= 2.0;
        if (t < 1.0)
            return -0.5 * (std::sqrt(1.0 - t * t) - 1.0);
        t -= 2.0;
        return 0.5 * (std::sqrt(1.0 - t * t) + 1.0);
    }


    // ===================================================================================
    // Elastic Easing
    // ===================================================================================

    /**
     * @brief Elastic ease-in function.
     *
     * Accelerates exponentially with overshoot and oscillation, starting from zero velocity.
     *
     * @param t Normalized time in the range [0.0, 1.0].
     * @param a Amplitude of the oscillation (default is 1.0).
     * @param p Period of the oscillation (default is 0.3).
     * @return Eased value in the same range.
     */
    static constexpr Float64 InElastic(Float64 t, Float64 a = 1.0, Float64 p = 0.3) noexcept
    {
        if (t == 0.0)
            return 0.0;
        if (t == 1.0)
            return 1.0;
        Float64 s;
        if (a < 1.0)
        {
            a = 1.0;
            s = p / 4.0;
        } else
            s = p / (2.0 * M_PI) * std::asin(1.0 / a);
        t -= 1.0;
        return -(a * std::pow(2.0, 10.0 * t) * std::sin((t - s) * (2.0 * M_PI) / p));
    }

    /**
     * @brief Elastic ease-out function.
     *
     * Decelerates exponentially with overshoot and oscillation, ending at zero velocity.
     *
     * @param t Normalized time in the range [0.0, 1.0].
     * @param a Amplitude of the oscillation (default is 1.0).
     * @param p Period of the oscillation (default is 0.3).
     * @return Eased value in the same range.
     */
    static constexpr Float64 OutElastic(Float64 t, Float64 a = 1.0, Float64 p = 0.3) noexcept
    {
        if (t == 0.0)
            return 0.0;
        if (t == 1.0)
            return 1.0;
        Float64 s;
        if (a < 1.0)
        {
            a = 1.0;
            s = p / 4.0;
        } else
            s = p / (2.0 * M_PI) * std::asin(1.0 / a);
        return a * std::pow(2.0, -10.0 * t) * std::sin((t - s) * (2.0 * M_PI) / p) + 1.0;
    }

    /**
     * @brief Elastic ease-in-out function.
     *
     * Accelerates then decelerates with exponential oscillations and overshoot.
     *
     * @param t Normalized time in the range [0.0, 1.0].
     * @param a Amplitude of the oscillation (default is 1.0).
     * @param p Period of the oscillation (default is 0.45).
     * @return Eased value in the same range.
     */
    static constexpr Float64 InOutElastic(Float64 t, Float64 a = 1.0, Float64 p = 0.45) noexcept
    {
        if (t == 0.0)
            return 0.0;
        if (t == 1.0)
            return 1.0;
        Float64 s;
        if (a < 1.0)
        {
            a = 1.0;
            s = p / 4.0;
        } else
            s = p / (2.0 * M_PI) * std::asin(1.0 / a);
        t *= 2.0;
        if (t < 1.0)
        {
            t -= 1.0;
            return -0.5 * (a * std::pow(2.0, 10.0 * t) * std::sin((t - s) * (2.0 * M_PI) / p));
        }
        t -= 1.0;
        return 0.5 * (a * std::pow(2.0, -10.0 * t) * std::sin((t - s) * (2.0 * M_PI) / p)) + 1.0;
    }
};

#endif // CZ_CZEASE_H
