#ifndef CZ_CZPOINTERSCROLLEVENT_H
#define CZ_CZPOINTERSCROLLEVENT_H

#include <CZ/Events/CZInputEvent.h>
#include <CZ/skia/core/SkPoint.h>

class CZ::CZPointerScrollEvent : public CZInputEvent
{
public:
    CZ_EVENT_DECLARE_COPY

    /**
     * @brief Source of a scroll event
     *
     * Possible sources of a scroll event.
     */
    enum Source : UInt32
    {
        /// [Mouse wheel (120 value)](https://wayland.freedesktop.org/libinput/doc/latest/api/group__event__pointer.html#ga31d3c555e912f021d3880d1cd7eb8a49)
        Wheel = 0,

        /// [Finger (continuous)](https://wayland.freedesktop.org/libinput/doc/latest/api/group__event__pointer.html#ga64ae33acadd4daf2144b906878f64882)
        Finger = 1,

        /// [Continuous movement (with unspecified source)](https://wayland.freedesktop.org/libinput/doc/latest/api/group__event__pointer.html#ga64ae33acadd4daf2144b906878f64882)
        Continuous = 2,

        /// [Side movement of a mouse wheel](https://wayland.freedesktop.org/libinput/doc/latest/api/group__device.html#gga76c012d8f6d7656fb795dc7bdf9d6551a60e7f4a41ceda06fe3eba2d512dc8ec9)
        WheelTilt = 3,

        /// [Legacy mouse wheel (discrete)](https://wayland.freedesktop.org/libinput/doc/latest/api/group__event__pointer.html#ga38d12fc6884c9943e261febdb2384b98)
        WheelLegacy = 1000
    };

    /**
     * @brief Relative directional information of the entity causing the axis motion.
     */
    enum RelativeDirection : UInt8
    {
        /// The scroll direction matches the physical movement (e.g., fingers move down, scroll moves down).
        Identical = static_cast<UInt8>(0),

        /// The scroll direction is reversed due to natural scrolling (e.g., fingers move down, scroll moves up).
        Inverted = static_cast<UInt8>(1)
    };

    CZPointerScrollEvent() noexcept : CZInputEvent(Type::PointerScroll) {};

    bool hasX {};
    bool hasY {};
    RelativeDirection relativeDirectionX {};
    RelativeDirection relativeDirectionY {};
    SkPoint axes {};
    SkIPoint axesDiscrete {};
    Source source {};
};

#endif // CZ_CZPOINTERSCROLLEVENT_H
