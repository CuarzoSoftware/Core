#ifndef CZ_CZTOUCHFRAMEEVENT_H
#define CZ_CZTOUCHFRAMEEVENT_H

#include <CZ/Core/Events/CZInputEvent.h>

/**
 * @brief Marks the end of a set of touch events that belong together (sent after a group of down/move/up events).
 */
class CZ::CZTouchFrameEvent : public CZInputEvent
{
public:
    CZ_EVENT_DECLARE_COPY
    CZTouchFrameEvent() noexcept : CZInputEvent(Type::TouchFrame) {};
};

#endif // CZ_CZTOUCHFRAMEEVENT_H
