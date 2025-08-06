#ifndef CZ_CZINPUTDEVICEPLUGGEDEVENT_H
#define CZ_CZINPUTDEVICEPLUGGEDEVENT_H

#include <CZ/Events/CZEvent.h>
#include <memory>

class CZ::CZInputDevicePluggedEvent : public CZEvent
{
public:
    CZ_EVENT_DECLARE_COPY
    CZInputDevicePluggedEvent() noexcept : CZEvent(Type::InputDevicePlugged) {};
    std::shared_ptr<CZInputDevice> device;
};

#endif // CZ_CZINPUTDEVICEPLUGGEDEVENT_H
