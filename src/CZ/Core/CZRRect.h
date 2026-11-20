#ifndef CZ_CZRRECT_H
#define CZ_CZRRECT_H

#include <CZ/skia/core/SkRect.h>
#include <CZ/Core/CZCore.h>

/**
 * @brief Integer rectangle with a radius for each corner.
 *
 * Extends SkIRect with corner radii, ordered clockwise from the top-left corner.
 */
struct CZ::CZRRect : public SkIRect
{
public:
    /** @brief Creates a rounded rectangle from @p rect and the corner radii. */
    CZRRect(const SkIRect &rect = SkIRect::MakeEmpty(), Int32 radTL = 0, Int32 radTR = 0, Int32 radBR = 0, Int32 radBL = 0) noexcept :
        SkIRect(rect), fRadTL(radTL), fRadTR(radTR), fRadBR(radBR), fRadBL(radBL)
    {}

    /**
     * @brief Whether the rectangle and radii are consistent.
     *
     * The size and radii must be non-negative, and the radii of adjacent corners must not add up to
     * more than the width or height of the side they share.
     */
    bool isValid() const noexcept
    {
        return
            width() >= 0 &&
            height() >= 0 &&
            fRadTL >= 0 &&
            fRadTR >= 0 &&
            fRadBR >= 0 &&
            fRadBL >= 0 &&
            fRadTL + fRadTR <= width() &&
            fRadBL + fRadBR <= width() &&
            fRadTL + fRadBL <= height() &&
            fRadTR + fRadBR <= height();
    }

    /** @brief Equal if the rectangles and all four radii are equal. */
    constexpr bool operator==(const CZRRect& other) const
    {
        return fLeft == other.fLeft  &&
               fRight == other.fRight  &&
               fBottom == other.fBottom  &&
               fTop == other.fTop  &&
               fRadTL == other.fRadTL &&
               fRadTR == other.fRadTR &&
               fRadBR == other.fRadBR &&
               fRadBL == other.fRadBL;
    }

    /** @brief Negation of operator==(). */
    constexpr bool operator!=(const CZRRect& other) const
    {
        return !(operator==(other));
    }

    Int32 fRadTL { 0 }; ///< Top-left radius.
    Int32 fRadTR { 0 }; ///< Top-right radius.
    Int32 fRadBR { 0 }; ///< Bottom-right radius.
    Int32 fRadBL { 0 }; ///< Bottom-left radius.
};

#endif // CZ_CZRRECT_H
