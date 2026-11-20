#ifndef CZ_CZINPUTEVENT_H
#define CZ_CZINPUTEVENT_H

#include <CZ/Core/Events/CZEvent.h>
#include <memory>

/**
 * @brief Base class for pointer, keyboard and touch events.
 */
class CZ::CZInputEvent : public CZEvent
{
public:
    /** @brief Constructs an input event of the given type. */
    CZInputEvent(Type type) noexcept : CZEvent(type){}
    /** @brief Timestamp in milliseconds (CZTime::Ms() at creation, or the input device's timestamp). */
    UInt32 ms { CZTime::Ms() };
    /** @brief Timestamp in microseconds (CZTime::Us() at creation, or the input device's timestamp). */
    UInt64 us { CZTime::Us() };
    /** @brief The input device that generated the event, if known. */
    std::shared_ptr<CZInputDevice> device;
};

#endif // CZ_CZINPUTEVENT_H
