#ifndef CZ_CZANIMATION_H
#define CZ_CZANIMATION_H

#include <CZ/Core/CZObject.h>
#include <CZ/Core/CZSignal.h>
#include <chrono>
#include <functional>

/**
 * @brief Base class for animations.
 */
class CZ::CZAnimation : public CZObject
{
public:
    /**
     * @brief Callback function type used for `onUpdate()` and `onFinish()` events.
     */
    using Callback = std::function<void(CZAnimation*)>;

    /**
     * @brief Destructor.
     *
     * @note If the animation is destroyed before it finishes, the `onFinish` callback
     *       will not be invoked.
     */
    ~CZAnimation() noexcept;

    /**
     * @brief Emitted on every animation step, after the value has been updated. Carries this animation.
     *
     * Subscribe with `onUpdate.subscribe(owner, callback)`. A callback may also be passed to the
     * constructor (it is subscribed here with the animation itself as the owner).
     */
    CZSignal<CZAnimation*> onUpdate;

    /**
     * @brief Emitted once when the animation comes to rest (never on `pause()`). Carries this animation.
     */
    CZSignal<CZAnimation*> onFinish;

    /**
     * @brief Returns the current animation value.
     *
     * @note The range depends on the specific animation type.
     */
    Float64 value() const noexcept { return m_value; }

    /**
     * @brief Returns whether the animation is currently running.
     */
    bool isRunning() const noexcept { return m_isRunning; }

    /**
     * @brief Starts the animation.
     *
     * @note This has no effect if the animation is already running. To (re)start from the beginning
     *       while running, use reset().
     */
    void start() noexcept;

    /**
     * @brief Restarts the animation from the beginning, even if it is already running.
     *
     * Resets the timeline and the value to its start (via onStart(): 0 for CZLinearAnimation, the
     * `from` point for CZSpringAnimation) and keeps it running. Useful to re-trigger a transition
     * after its endpoints changed, from the current intermediate state, without waiting for it to
     * finish.
     */
    virtual void reset() noexcept;

    /**
     * @brief Pauses the animation.
     *
     * @details Stops the animation immediately without invoking the `onFinish`
     *          callback. The current state is preserved and will remain unchanged
     *          until `start()` is called again.
     */
    void pause() noexcept;

    /**
     * @brief Returns the time point at which the animation was last started.
     *
     * @note This value is updated when `start()` is called.
     */
    std::chrono::steady_clock::time_point startTime() const noexcept { return m_startTime; }

protected:
    CZAnimation(Callback onUpdate, Callback onFinish, bool oneshot) noexcept;
    virtual void onStart() noexcept = 0;
    // Advances the animation state for the current frame (was onUpdate(); renamed to free the name
    // for the onUpdate signal). Implemented by the concrete animation types.
    virtual void advance() noexcept = 0;
    Float64 m_value { 0.0 };
    bool m_isRunning { false };
private:
    friend class CZCore;
    std::chrono::steady_clock::time_point m_startTime;
    bool m_processed { false };
    bool m_pendingDestroy { false };
    bool m_oneshot { false };
};

#endif // CZ_CZANIMATION_H
