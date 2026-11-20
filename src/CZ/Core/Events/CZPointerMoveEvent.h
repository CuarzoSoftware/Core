#ifndef CZ_CZPOINTERMOVEEVENT_H
#define CZ_CZPOINTERMOVEEVENT_H

#include <CZ/skia/core/SkPoint.h>
#include <CZ/Core/Events/CZInputEvent.h>

/**
 * @brief The pointer moved.
 */
class CZ::CZPointerMoveEvent : public CZInputEvent
{
public:
    CZ_EVENT_DECLARE_COPY
    CZPointerMoveEvent() noexcept : CZInputEvent(Type::PointerMove) {}
    /** @brief Pointer position. Can be local to the target or global, depending on the context. */
    mutable SkPoint pos {};
    /** @brief Relative motion since the previous event, with pointer acceleration applied. */
    SkPoint delta {};
    /** @brief Relative motion since the previous event, without pointer acceleration. */
    SkPoint deltaUnaccelerated {};
};

#endif // CZ_CZPOINTERMOVEEVENT_H
