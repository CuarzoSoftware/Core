#ifndef CZREGIONUTILS_H
#define CZREGIONUTILS_H

#include <CZ/CZTransform.h>

class CZ::CZRegionUtils
{
public:

    const static SkRegion &Empty() noexcept;

    /**
     * @brief Applies the specified transform to all rectangles within the given size.
     *
     * This function clips the rectangles within the specified size and applies the specified transform.
     *
     * @param size The size used for clipping and transformation.
     * @param transform The transform to be applied to the rectangles.
     */
    static void ApplyTransform(SkRegion &region, SkISize size, CZTransform transform) noexcept;

    /**
     * @brief Multiplies the components of each rectangle in the region by the given factor.
     *
     * @param xFactor The x-axis factor to multiply by.
     * @param yFactor The y-axis factor to multiply by.
     */
    static void Scale(SkRegion &region, Float32 xFactor, Float32 yFactor) noexcept;

    /**
     * @brief Multiplies the components of each rectangle in the region by the given factor.
     *
     * @param factor The factor to multiply by.
     */
    static void Scale(SkRegion &region, Float32 factor) noexcept;
    static void Scale(const SkRegion &src, SkRegion &dst, Float32 factor) noexcept;

    /**
     * @brief Returns the point within the region closest to the given point.
     *
     * If the point is inside the region or the region is empty, the same point is returned.
     *
     * @param point The point to find the closest point within the region.
     * @param padding Optional padding applied to each of the region rectangles.
     */
    static SkPoint ClosestPointFrom(const SkRegion &region, SkPoint point, Float32 padding = 0.f) noexcept;
};

#endif // CZREGIONUTILS_H
