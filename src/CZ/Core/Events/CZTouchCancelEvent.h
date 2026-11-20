#ifndef CZ_CZTOUCHCANCELEVENT_H
#define CZ_CZTOUCHCANCELEVENT_H

#include <CZ/Core/Events/CZInputEvent.h>

/**
 * @brief The current touch session was cancelled (all active touch points are no longer valid).
 */
class CZ::CZTouchCancelEvent : public CZInputEvent
{
public:
    CZ_EVENT_DECLARE_COPY
    CZTouchCancelEvent() noexcept : CZInputEvent(Type::TouchCancel) {};
};

#endif // CZ_CZTOUCHCANCELEVENT_H
