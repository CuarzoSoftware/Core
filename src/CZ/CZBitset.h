#ifndef CZBITSET_H
#define CZBITSET_H

#include <CZ/CZ.h>
#include <type_traits>

namespace CZ
{
    template<typename, bool>
    struct _numerical_underlying_type{};

    template<typename T>
    struct _numerical_underlying_type<T, true>
    {
        using type = std::underlying_type<T>::type;
    };

    template<typename T>
    struct _numerical_underlying_type<T, false>
    {
        using type = T;
    };

    template<typename T>
    struct numerical_underlying_type : _numerical_underlying_type<T, std::is_enum<T>::value> {};
}

/**
 * @brief Compact way of storing and managing conditions or states
 *
 * The CZBitset class template is similar to `std::bitset` in that it enables the compact storage of a set of conditions or states using bits.\n
 * Unlike `std::bitset`, CZBitset functions are designed to modify and retrieve bit states using flags rather than indices, which are ideally
 * defined within an enum.
 *
 * It is widely used in the private API of AK classes to optimize memory usage.
 *
 * Example usage:
 *
 * @code
 * enum MyFlags
 * {
 *      FlagA = 1 << 0,
 *      FlagB = 1 << 1,
 *      FlagC = 1 << 2
 * };
 *
 * CZBitset<MyFlags> myFlags;
 *
 * myFlags.add(MyFlags::FlagA | MyFlags::FlagC);
 *
 * if (myFlags.check(MyFlags::FlagC))
 * {
 *      // do something
 * }
 * @endcode
 */

template <class T>
class CZ::CZBitset
{
    static_assert(std::is_integral<T>::value || std::is_enum<T>::value, "Type is not integral or enum.");
    using Flag = CZ::numerical_underlying_type<T>::type;

public:

    /**
     * @brief Constructor for CZBitset
     *
     * Initializes the bitset with the specified initial flags. If no flags are provided, the bitset is constructed empty without any flags.
     *
     * @param flags Initial flags to set (default is 0)
     */
    constexpr CZBitset(Flag flags = 0) noexcept : m_flags(flags) {}

    /**
     * @brief Add new flags to the bitfield
     *
     * Sets the specified flags by combining them with the existing flags using the bitwise OR operator '|'.
     *
     * @param flags The flag or combination of flags to be added
     */
    constexpr void add(Flag flags) noexcept
    {
        m_flags |= flags;
    }

    /**
     * @brief Remove flags from the bitfield
     *
     * Clears the specified flags by performing a bitwise AND operation with the complement of the provided flags.
     *
     * @param flags The flag or combination of flags to be removed
     */
    constexpr void remove(Flag flags) noexcept
    {
        m_flags &= ~flags;
    }

    constexpr bool isEmpty() const noexcept
    {
        return m_flags == 0;
    }

    /**
     * @brief Check if at least one flag exists
     *
     * Checks if at least one of the specified flags is set in the bitfield.
     *
     * @param flags The flag or combination of flags to be checked
     * @return `true` if at least one flag is set, otherwise `false`
     */
    constexpr bool has(Flag flags) const noexcept
    {
        return (m_flags & flags) != 0;
    }

    /**
     * @brief Check if all specified flags exist
     *
     * Checks if all of the specified flags are set in the bitfield.
     *
     * @param flags The flag or combination of flags to be checked
     * @return `true` if all specified flags are set, otherwise `false`
     */
    constexpr bool hasAll(Flag flags) const noexcept
    {
        return (m_flags & flags) == flags;
    }

    /**
     * @brief Gets the current set of flags
     *
     * Retrieves the current set of flags stored in the bitfield.
     *
     * @return The current set of flags
     */
    constexpr Flag get() const noexcept
    {
        return m_flags;
    }

    /**
     * @brief Set new flags in the bitfield
     *
     * Replaces the current set of flags in the bitfield with the specified flags.
     *
     * @param flags The new flag or combination of flags to be set
     */
    constexpr void set(Flag flags) noexcept
    {
        m_flags = flags;
    }

    /**
     * @brief Set or unset a specific flag in the bitfield
     *
     * Modifies the specified flag (or flags) in the bitfield. If `enable` is `true`, the flag is set, otherwise, it is removed.
     *
     * @param flag The flag to be set or unset
     * @param enable If `true`, set the flag, if `false`, remove the flag
     */
    constexpr void setFlag(Flag flag, bool enable) noexcept
    {
        if (enable)
            add(flag);
        else
            remove(flag);
    }

    /**
     * @brief Performs a bitwise OR operation with another CZBitset.
     *
     * @param flags The CZBitset containing flags to perform the operation with.
     * @return A reference to the modified CZBitset after the operation.
     */
    constexpr CZBitset<T>& operator|=(T flags) noexcept
    {
        m_flags |= Flag(flags);
        return *this;
    }

    /**
     * @brief Performs a bitwise AND operation with another CZBitset.
     *
     * @param flags The CZBitset containing flags to perform the operation with.
     * @return A reference to the modified CZBitset after the operation.
     */
    constexpr CZBitset<T>& operator&=(T flags) noexcept
    {
        m_flags &= Flag(flags);
        return *this;
    }

    /**
     * @brief Performs a bitwise XOR operation with another CZBitset.
     *
     * @param flags The CZBitset containing flags to perform the operation with.
     * @return A reference to the modified CZBitset after the operation.
     */
    constexpr CZBitset<T>& operator^=(T flags) noexcept
    {
        m_flags ^= Flag(flags);
        return *this;
    }

    friend constexpr T operator~(T flag) noexcept
    {
        return T(~Flag(flag));
    }

    friend constexpr T operator&(T a, T b) noexcept
    {
        return T(Flag(a) & Flag(b));
    }

    friend constexpr T operator|(T a, T b) noexcept
    {
        return T(Flag(a) | Flag(b));
    }

    friend constexpr T operator^(T a, T b) noexcept
    {
        return T(Flag(a) ^ Flag(b));
    }

    constexpr operator T() const noexcept
    {
        return T(m_flags);
    }

    constexpr CZBitset<T> operator ~() const noexcept
    {
        return CZBitset<T>(~m_flags);
    }

private:
    Flag m_flags;
};

#endif
