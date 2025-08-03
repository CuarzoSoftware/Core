#ifndef CZMATHUTILS_H
#define CZMATHUTILS_H

#include <CZ/Cuarzo.h>
#include <cassert>
#include <type_traits>

class CZ::CZMathUtils
{
public:
    template <typename T>
    requires std::is_integral_v<T> && std::is_unsigned_v<T>
    static constexpr T DivCeil(T dividend, T divisor) noexcept
    {
        assert(divisor != 0);
        return (dividend + divisor - 1) / divisor;
    }
};


#endif // CZMATHUTILS_H
