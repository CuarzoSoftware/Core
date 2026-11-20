#ifndef CZ_CZSETUTILS_H
#define CZ_CZSETUTILS_H

#include <CZ/Core/Cuarzo.h>

/**
 * @brief Helper functions for set containers (e.g. `std::set`, `std::unordered_set`).
 */
class CZ::CZSetUtils
{
public:
    /** @brief Returns the elements present in both @p a and @p b. */
    template <typename Set>
    static inline Set Intersect(const Set &a, const Set &b)
    {
        Set result;
        const auto &smaller = (a.size() < b.size()) ? a : b;
        const auto &larger  = (a.size() < b.size()) ? b : a;

        for (const auto& item : smaller)
            if (larger.find(item) != larger.end())
                result.insert(item);

        return result;
    }

    /** @brief Returns the elements present in @p a, @p b or both. */
    template <typename Set>
    static inline Set Union(const Set &a, const Set &b)
    {
        Set result;
        result.insert(a.begin(), a.end());
        result.insert(b.begin(), b.end());
        return result;
    }
};

#endif // CZ_CZSETUTILS_H
