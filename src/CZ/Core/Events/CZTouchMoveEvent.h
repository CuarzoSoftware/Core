#ifndef CZ_CZTOUCHMOVEEVENT_H
#define CZ_CZTOUCHMOVEEVENT_H

#include <CZ/Core/Events/CZInputEvent.h>
#include <CZ/skia/core/SkPoint.h>

/**
 * @brief A touch point moved.
 */
class CZ::CZTouchMoveEvent : public CZInputEvent
{
public:
    CZ_EVENT_DECLARE_COPY
    CZTouchMoveEvent() noexcept : CZInputEvent(Type::TouchMove) {};
    /** @brief Position local to the target (e.g. surface-local coordinates in Louvre, set by the compositor). */
    mutable SkPoint localPos {};
    /** @brief Touch point identifier (the touch slot), shared by the down, move and up events of a touch point. */
    Int32 id {};
    /** @brief Position normalized to the touch device area, in the range [0, 1]. */
    SkPoint pos {};
};

#endif // CZ_CZTOUCHMOVEEVENT_H
