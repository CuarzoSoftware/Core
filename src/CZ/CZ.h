#ifndef CZ_CZ_H
#define CZ_CZ_H

#include <cstdint>

#define CZ_UNUSED(object){(void)object;}
#define CZ_DISABLE_COPY(ClassName) \
    ClassName(const ClassName&) = delete; \
    ClassName(ClassName&&) = delete; \
    ClassName& operator=(const ClassName&) = delete; \
    ClassName& operator=(ClassName&&) = delete;

namespace CZ
{
    class CZObject;
    class CZEvent;
    class CZWeakUtils;
    template <class T> class CZWeak;
    template <class T> class CZBitset;
    class CZSignalBase;
    template<typename...Args> class CZSignal;
    class CZListener;
    template<typename...Args> class CZListenerTemplate;

    class CZRegionUtils;

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

class SkRect;
class SkIRect;
class SkPoint;
class SkIPoint;
class SkSize;
class SkISize;
class SkRegion;

#endif
