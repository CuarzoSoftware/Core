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
 * If @ref CZPresentationEvent::discarded is set to @c true, the data in @ref CZPresentationEvent::info is invalid,
 * except for `CZPresentationTime::paintEventId`, which remains meaningful.
 */
class CZ::CZPresentationEvent : public CZEvent
{
public:
    CZ_EVENT_DECLARE_COPY
    CZPresentationEvent() noexcept : CZEvent(Type::Presentation) {};

    /** @brief Presentation details (timestamp, refresh period, etc.). */
    CZPresentationTime info {};
    /** @brief `true` if the content was discarded instead of presented. */
    bool discarded { false };
};

#endif // CZ_CZPRESENTATIONEVENT_H
