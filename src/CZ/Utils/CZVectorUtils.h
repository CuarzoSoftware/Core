#ifndef CZVECTORUTILS_H
#define CZVECTORUTILS_H

#include <CZ/CZ.h>
#include <vector>
#include <algorithm>

class CZ::CZVectorUtils
{
public:
    template <typename T>
    static inline bool RemoveOneUnordered(std::vector<T>& vec, T val) noexcept
    {
        auto it { std::find(vec.begin(), vec.end(), val) };

        if (it == vec.end())
            return false;

        *it = std::move(vec.back());
        vec.pop_back();
        return true;
    }
};

#endif // CZVECTORUTILS_H
