#ifndef CZ_CZINPUTEVENT_H
#define CZ_CZINPUTEVENT_H

#include <CZ/Events/CZEvent.h>
#include <memory>

class CZ::CZInputEvent : public CZEvent
{
public:
    CZInputEvent(Type type) noexcept : CZEvent(type){}
    UInt32 ms { CZTime::Ms() };
    UInt64 us { CZTime::Us() };
    std::shared_ptr<CZInputDevice> device;
};

#endif // CZ_CZINPUTEVENT_H
