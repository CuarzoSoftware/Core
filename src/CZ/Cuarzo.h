#ifndef CZ_CUARZO_H
#define CZ_CUARZO_H

#include <cstdint>

#define CZ_UNUSED(object){(void)object;}
#define CZ_DISABLE_COPY(ClassName) \
    ClassName(const ClassName&) = delete; \
    ClassName(ClassName&&) = delete; \
    ClassName& operator=(const ClassName&) = delete; \
    ClassName& operator=(ClassName&&) = delete;

namespace CZ
{
    class CZCore;
    class CZObject;
    class CZTime;
    class CZInputDevice;
    class CZEventSource;
    class CZBooleanEventSource;
    class CZTimer;
    class CZSafeEventQueue;
    class CZWeakUtils;
    template <class T> class CZWeak;
    template <class T> class CZBitset;
    class CZSignalBase;
    template<typename...Args> class CZSignal;
    class CZListener;
    template<typename...Args> class CZListenerTemplate;
    class CZLogger;

    class CZSharedMemory;
    class CZRegionUtils;
    class CZStringUtils;
    class CZVectorUtils;
    class CZMathUtils;
    class CZSetUtils;

    class CZEvent;
    class CZDestroyEvent;
    class CZCloseEvent;
    class CZDestroyEvent;
    class CZLayoutEvent;
    class CZInputEvent;    

    class CZPointerEnterEvent;
    class CZPointerMoveEvent;
    class CZPointerScrollEvent;
    class CZPointerButtonEvent;
    class CZPointerLeaveEvent;
    class CZPointerSwipeBeginEvent;
    class CZPointerSwipeUpdateEvent;
    class CZPointerSwipeEndEvent;
    class CZPointerPinchBeginEvent;
    class CZPointerPinchUpdateEvent;
    class CZPointerPinchEndEvent;
    class CZPointerHoldBeginEvent;
    class CZPointerHoldEndEvent;

    class CZKeyboardEnterEvent;
    class CZKeyboardLeaveEvent;
    class CZKeyboardKeyEvent;
    class CZKeyboardModifiersEvent;

    class CZTouchDownEvent;
    class CZTouchUpEvent;
    class CZTouchMoveEvent;
    class CZTouchFrameEvent;
    class CZTouchCancelEvent;

    class CZInputDevicePluggedEvent;
    class CZInputDeviceUnpluggedEvent;

    class CZWindowStateEvent;

    /// @brief 64 bits unsigned integer
    typedef uint64_t UInt64;

    /// @brief 64 bits signed integer
    typedef int64_t  Int64;

    /// @brief 32 bits unsigned integer
    typedef uint32_t UInt32;

    /// @brief 32 bits signed integer
    typedef int32_t  Int32;

    /// @brief 16 bits unsigned integer
    typedef uint16_t UInt16;

    /// @brief 16 bits signed integer
    typedef int16_t  Int16;

    /// @brief 8 bits unsigned integer
    typedef uint8_t  UInt8;

    /// @brief 8 bits signed integer
    typedef int8_t   Int8;

    /// @brief 64 bits float
    typedef double   Float64;

    /// @brief 32 bits float
    typedef float    Float32;
}

struct SkPoint;
struct SkIPoint;
struct SkSize;
struct SkISize;
struct SkRect;
struct SkIRect;
class SkRegion;

#endif
