#ifndef CZ_CZTOUCHDOWNEVENT_H
#define CZ_CZTOUCHDOWNEVENT_H

#include <CZ/Core/Events/CZInputEvent.h>
#include <CZ/skia/core/SkPoint.h>

/**
 * @brief A new touch point appeared (a finger touched the screen).
 */
class CZ::CZTouchDownEvent : public CZInputEvent
{
public:
    CZ_EVENT_DECLARE_COPY
    CZTouchDownEvent() noexcept : CZInputEvent(Type::TouchDown) {};

    /** @brief Position local to the target (e.g. surface-local coordinates in Louvre, set by the compositor). */
    mutable SkPoint localPos {};
    /** @brief Position normalized to the touch device area, in the range [0, 1]. */
    SkPoint pos {};
    /** @brief Touch point identifier (the touch slot), shared by the down, move and up events of a touch point. */
    Int32 id {};
};

#endif // CZ_CZTOUCHDOWNEVENT_H
