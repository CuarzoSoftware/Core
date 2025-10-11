#ifndef CZINPUTDEVICEUNPLUGGEDEVENT_H
#define CZINPUTDEVICEUNPLUGGEDEVENT_H

#include <CZ/Core/Events/CZEvent.h>
#include <memory>

class CZ::CZInputDeviceUnpluggedEvent : public CZEvent
{
public:
    CZ_EVENT_DECLARE_COPY
    CZInputDeviceUnpluggedEvent() noexcept : CZEvent(Type::InputDeviceUnplugged) {};
    std::shared_ptr<CZInputDevice> device;
};

#endif // CZINPUTDEVICEUNPLUGGEDEVENT_H
