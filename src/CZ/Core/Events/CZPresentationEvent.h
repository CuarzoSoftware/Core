#ifndef CZ_CZPRESENTATIONEVENT_H
#define CZ_CZPRESENTATIONEVENT_H

#include <CZ/Core/Events/CZEvent.h>
#include <CZ/Core/CZPresentationTime.h>

/**
 * @brief Presentation time event.
 *
 * This event notifies that previously rendered content
 * has either been presented on screen or discarded.
 *
 * If @c discarded is set to @c true, the data in @ref info is invalid,
 * except for @ref CZPresentationTime::paintEventId, which remains meaningful.
 */
class CZ::CZPresentationEvent : public CZEvent
{
public:
    CZ_EVENT_DECLARE_COPY
    CZPresentationEvent() noexcept : CZEvent(Type::Presentation) {};

    CZPresentationTime info {};
    bool discarded { false };
};

#endif // CZ_CZPRESENTATIONEVENT_H
