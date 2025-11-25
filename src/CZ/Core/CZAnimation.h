#ifndef CZ_CZANIMATION_H
#define CZ_CZANIMATION_H

#include <CZ/Core/CZObject.h>
#include <chrono>

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
     * @brief Sets the `onUpdate` callback.
     *
     * @param onUpdate The callback function to invoke during updates, or `nullptr` to disable it.
     */
    void setOnUpdateCallback(Callback onUpdate) noexcept;

    /**
     * @brief Sets the `onFinish` callback.
     *
     * @param onFinish The callback function to invoke when the animation finishes, or `nullptr` to disable it.
     */
    void setOnFinishCallback(Callback onFinish) noexcept;

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
     * @note This has no effect if the animation is already running.
     */
    void start() noexcept;

    /**
     * @brief Stops the animation.
     *
     * @details Stops the animation immediately without invoking the `onFinish`
     *          callback. The current state is preserved and will remain unchanged
     *          until `start()` is called again.
     */
    void stop() noexcept;

    /**
     * @brief Returns the time point at which the animation was last started.
     *
     * @note This value is updated when `start()` is called.
     */
    std::chrono::steady_clock::time_point startTime() const noexcept { return m_startTime; }

protected:
    CZAnimation(Callback onUpdate, Callback onFinish, bool oneshot) noexcept;
    virtual void onStart() noexcept = 0;
    virtual void onUpdate() noexcept = 0;
    Float64 m_value { 0.0 };
    bool m_isRunning { false };
private:
    friend class CZCore;
    Callback m_onUpdate { nullptr };
    Callback m_onFinish { nullptr };
    std::chrono::steady_clock::time_point m_startTime;
    bool m_processed { false };
    bool m_pendingDestroy { false };
    bool m_oneshot { false };
};

#endif // CZ_CZANIMATION_H
