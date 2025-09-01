#ifndef CZ_CZTIME_H
#define CZ_CZTIME_H

#include <CZ/Core/Cuarzo.h>
#include <ctime>

/**
 * @brief Time utilities
 */
class CZ::CZTime
{
public:

    CZTime() = delete;

    /**
     * @brief Returns a new positive integer number each time it is called. Never 0.
     */
    static UInt32 NextSerial() noexcept;

    /**
     * @brief Milliseconds
     *
     * Monotonic time with a granularity of milliseconds and an undefined base.
     */
    static UInt32 Ms() noexcept
    {
        timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        return static_cast<UInt32>(ts.tv_sec) * 1000 + static_cast<UInt32>(ts.tv_nsec) / 1000000;
    }

    /**
     * @brief Microseconds
     *
     * Monotonic time with a granularity of microseconds and an undefined base.
     */
    static UInt64 Us() noexcept
    {
        timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        return static_cast<UInt64>(ts.tv_sec) * 1000000 + static_cast<UInt64>(ts.tv_nsec) / 1000;
    }

    /**
     * @brief Nanoseconds
     *
     * Monotonic time with nanosecond granularity and undefined base.
     */
    static timespec Ns() noexcept
    {
        timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        return ts;
    }
};

#endif // CZ_CZTIME_H
