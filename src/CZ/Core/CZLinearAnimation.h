#ifndef CZ_CZLINEARANIMATION_H
#define CZ_CZLINEARANIMATION_H

#include <CZ/Core/CZAnimation.h>
/**
 * @brief Time-based animation utility.
 *
 * This class provides a mechanism for animating object properties such as position, color, opacity, and more.
 * It runs for a fixed duration, specified in milliseconds.
 *
 * Once started, the `onUpdate()` callback is invoked periodically, allowing access to the `value()` property,
 * a 64-bit floating-point number linearly interpolated from 0.0 to 1.0 representing the animation's progress.
 *
 * When the animation completes, the `onFinish()` callback is triggered. At that point, `value()` will return 1.0.
 */
class CZ::CZLinearAnimation : public CZAnimation
{
public:

    /**
     * @brief Creates a reusable animation.
     *
     * Creates an animation without starting it immediately.
     *
     * @param durationMs The duration of the animation in milliseconds.
     * @param onUpdate A callback function triggered each time the value() property changes. `nullptr` can be passed if not used.
     * @param onFinish A callback function triggered once the value() property reaches 1.0. `nullptr` can be passed if not used.
     */
    CZLinearAnimation(UInt32 durationMs = 0, Callback onUpdate = nullptr, Callback onFinish = nullptr) noexcept;

    /**
     * @brief Creates and launches a one-time animation with automatic cleanup.
     *
     * The oneShot() method creates and starts an animation immediately, and it is automatically destroyed once finished.
     *
     * @param durationMs The duration of the animation in milliseconds.
     * @param onUpdate A callback function triggered each time the value() property changes. `nullptr` can be passed if not used.
     * @param onFinish A callback function triggered once the value() property reaches 1.0. `nullptr` can be passed if not used.
     */
    static void OneShot(UInt32 durationMs, Callback onUpdate = nullptr, Callback onFinish = nullptr) noexcept;

    /**
     * @brief Sets the duration of the animation in milliseconds.
     *
     * Use this method to specify the duration of the animation in milliseconds.
     *
     * @note It is not permissible to invoke this method while the animation is in progress, and attempting to do so will yield no results.
     *
     * @param durationMs The duration of the animation in milliseconds.
     */
    void setDuration(UInt32 durationMs) noexcept
    {
        if (isRunning())
            return;

        m_duration = durationMs;
    }

    /**
     * @brief Returns the duration of the animation in milliseconds.
     *
     * Use this method to retrieve the duration of the animation in milliseconds.
     *
     * @return The duration of the animation in milliseconds.
     */
    UInt32 duration() const noexcept { return m_duration; }

protected:
    CZLinearAnimation(bool oneshot, UInt32 durationMs = 0, Callback onUpdate = nullptr, Callback onFinish = nullptr) noexcept;
    void onStart() noexcept override;
    void onUpdate() noexcept override;

private:
    Int64 m_duration;
};
#endif // CZ_CZLINEARANIMATION_H
