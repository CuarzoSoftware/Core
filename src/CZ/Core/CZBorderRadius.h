#ifndef CZ_CZBORDERRADIUS_H
#define CZ_CZBORDERRADIUS_H

#include <CZ/Core/Cuarzo.h>
#include <algorithm>

/**
 * @brief The four corner radii of a rounded rectangle.
 *
 * Corners are ordered clockwise from the top-left: top-left, top-right, bottom-right, bottom-left
 * (the same order as CSS `border-radius` and SkRRect's corner enum).
 */
struct CZ::CZBorderRadius
{
    /** @brief The same radius for all four corners. */
    constexpr CZBorderRadius(Int32 all = 0) noexcept :
        fTL(all), fTR(all), fBR(all), fBL(all) {}

    /** @brief Distinct radii, clockwise from the top-left corner. */
    constexpr CZBorderRadius(Int32 topLeft, Int32 topRight, Int32 bottomRight, Int32 bottomLeft) noexcept :
        fTL(topLeft), fTR(topRight), fBR(bottomRight), fBL(bottomLeft) {}

    /** @brief The same radius for all four corners. */
    static constexpr CZBorderRadius Make(Int32 rad) noexcept
    {
        return CZBorderRadius(rad);
    }

    /** @brief One radius for the top corners, another for the bottom corners. */
    static constexpr CZBorderRadius MakeTB(Int32 top, Int32 bottom) noexcept
    {
        return CZBorderRadius(top, top, bottom, bottom);
    }

    /** @brief One radius for the left corners, another for the right corners. */
    static constexpr CZBorderRadius MakeLR(Int32 left, Int32 right) noexcept
    {
        return CZBorderRadius(left, right, right, left);
    }

    /** @brief Whether all four corners share the same radius. */
    constexpr bool isUniform() const noexcept
    {
        return fTL == fTR && fTR == fBR && fBR == fBL;
    }

    /** @brief The largest of the four radii. */
    constexpr Int32 max() const noexcept
    {
        return std::max(std::max(fTL, fTR), std::max(fBR, fBL));
    }

    constexpr bool operator==(const CZBorderRadius &other) const noexcept
    {
        return fTL == other.fTL && fTR == other.fTR && fBR == other.fBR && fBL == other.fBL;
    }

    constexpr bool operator!=(const CZBorderRadius &other) const noexcept
    {
        return !(*this == other);
    }

    Int32 fTL { 0 };
    Int32 fTR { 0 };
    Int32 fBR { 0 };
    Int32 fBL { 0 };
};

#endif // CZ_CZBORDERRADIUS_H
