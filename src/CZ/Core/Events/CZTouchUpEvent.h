#ifndef CZ_CZTOUCHUPEVENT_H
#define CZ_CZTOUCHUPEVENT_H

#include <CZ/Core/Events/CZInputEvent.h>
#include <CZ/skia/core/SkPoint.h>

/**
 * @brief A touch point was released (a finger was lifted).
 */
class CZ::CZTouchUpEvent : public CZInputEvent
{
public:
    CZ_EVENT_DECLARE_COPY
    CZTouchUpEvent() noexcept : CZInputEvent(Type::TouchUp) {};
    /** @brief Touch point identifier (the touch slot), shared by the down, move and up events of a touch point. */
    Int32 id {};
};

#endif // CZ_CZTOUCHUPEVENT_H
