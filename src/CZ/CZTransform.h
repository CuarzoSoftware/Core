#ifndef CZTRANSFORM_H
#define CZTRANSFORM_H

#include <CZ/CZ.h>

namespace CZ
{
    /**
     * @brief Transforms
     */
    enum class CZTransform : Int32
    {
        /// No transformation
        Normal = 0,

        /// Rotate 90 degrees counter-clockwise
        Rotated90 = 1,

        /// Rotate 180 degrees counter-clockwise
        Rotated180 = 2,

        /// Rotate 270 degrees counter-clockwise
        Rotated270 = 3,

        /// Flipped (swap left and right sides)
        Flipped = 4,

        /// Flip and rotate 90 degrees counter-clockwise
        Flipped90 = 5,

        /// Flip and rotate 180 degrees counter-clockwise
        Flipped180 = 6,

        /// Flip and rotate 270 degrees counter-clockwise
        Flipped270 = 7
    };

    /**
     * @brief Checks if the transformation results in swapping the width and height.
     *
     * @param transform The transformation to check.
     * @return `true` if the transformation includes a 90° or 270° rotation, `false` otherwise.
     */
    static inline constexpr bool Is90Transform(CZTransform transform) noexcept
    {
        return static_cast<Int32>(transform) & static_cast<Int32>(CZTransform::Rotated90);
    }

    /**
     * @brief Required transform to transition from transform 'a' to 'b'
     *
     * @param a The initial transform.
     * @param b The target transform.
     */
    static inline constexpr CZTransform RequiredTransform(CZTransform a, CZTransform b) noexcept
    {
        const Int32 bitmask { static_cast<Int32>(CZTransform::Rotated270) };
        const Int32 flip { (static_cast<Int32>(a) & ~bitmask) ^ (static_cast<Int32>(b) & ~bitmask) };
        Int32 rotation;

        if (flip)
            rotation = ((static_cast<Int32>(b) & bitmask) + (static_cast<Int32>(a) & bitmask)) & bitmask;
        else
        {
            rotation = (static_cast<Int32>(b) & bitmask) - (static_cast<Int32>(a) & bitmask);

            if (rotation < 0)
                rotation += 4;
        }

        return static_cast<CZTransform>(flip | rotation);
    }
}
#endif // CZTRANSFORM_H
