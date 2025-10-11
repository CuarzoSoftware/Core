#ifndef CZVECTORUTILS_H
#define CZVECTORUTILS_H

#include <CZ/Core/Cuarzo.h>
#include <vector>
#include <algorithm>

class CZ::CZVectorUtils
{
public:
    template <typename T>
    static inline void RemoveOne(std::vector<T>& vec, T val) noexcept
    {
        const auto it { std::find(vec.begin(), vec.end(), val) };
        if (it != vec.end())
            vec.erase(it);
    }

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

    template <typename T>
    static inline void DeleteAndPopBackAll(std::vector<T*>& vec) noexcept
    {
        while (!vec.empty())
        {
            delete vec.back();
            vec.pop_back();
        }
    }
};

#endif // CZVECTORUTILS_H
