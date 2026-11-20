#ifndef CZVECTORUTILS_H
#define CZVECTORUTILS_H

#include <CZ/Core/Cuarzo.h>
#include <vector>
#include <algorithm>

/**
 * @brief `std::vector` helper functions.
 */
class CZ::CZVectorUtils
{
public:
    /** @brief Removes the first element equal to @p val, if any, keeping the order of the rest. */
    template <typename T>
    static inline void RemoveOne(std::vector<T>& vec, T val) noexcept
    {
        const auto it { std::find(vec.begin(), vec.end(), val) };
        if (it != vec.end())
            vec.erase(it);
    }

    /**
     * @brief Removes the first element equal to @p val by replacing it with the last element (doesn't keep the order).
     *
     * @return `true` if an element was removed, `false` if not found.
     */
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

    /** @brief Deletes every element (with `delete`) from last to first and empties the vector. */
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
