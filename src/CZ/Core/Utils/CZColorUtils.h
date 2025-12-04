#ifndef CZCOLORUTILS_H
#define CZCOLORUTILS_H

#include <CZ/Core/Cuarzo.h>
#include <CZ/skia/core/SkColor.h>

class CZ::CZColorUtils
{
public:
    /**
     * @brief Linearly combines two SkColors as A*t + B*(1 - t).
     *
     * @param cA   The first color (weight = t).
     * @param cB   The second color (weight = 1 - t).
     * @param t    Blend factor in the range [0, 1].
     * @return     The blended SkColor.
     */
    static constexpr SkColor Mix(SkColor cA, SkColor cB, SkScalar t) noexcept
    {
        t = (t < 0.0f) ? 0.0f : (t > 1.0f ? 1.0f : t); // Clamp t to [0, 1]

        const SkScalar inv { 1.0f - t };
        const UInt8 a = static_cast<UInt8>(SkColorGetA(cA) * t + SkColorGetA(cB) * inv);
        const UInt8 r = static_cast<UInt8>(SkColorGetR(cA) * t + SkColorGetR(cB) * inv);
        const UInt8 g = static_cast<UInt8>(SkColorGetG(cA) * t + SkColorGetG(cB) * inv);
        const UInt8 b = static_cast<UInt8>(SkColorGetB(cA) * t + SkColorGetB(cB) * inv);
        return SkColorSetARGB(a, r, g, b);
    }

    /**
     * @brief Linearly combines two SkColor4f values as A*t + B*(1 - t).
     *
     * @param cA   The first color (weight = t).
     * @param cB   The second color (weight = 1 - t).
     * @param t    Blend factor in the range [0, 1].
     * @return     The blended SkColor4f.
     */
    static constexpr SkColor4f Mix(SkColor4f cA, SkColor4f cB, SkScalar t) noexcept
    {
        t = (t < 0.0f) ? 0.0f : (t > 1.0f ? 1.0f : t); // Clamp t to [0, 1]
        const SkScalar inv { 1.0f - t };

        return SkColor4f{
            cA.fR * t + cB.fR * inv,
            cA.fG * t + cB.fG * inv,
            cA.fB * t + cB.fB * inv,
            cA.fA * t + cB.fA * inv
        };
    }
};

#endif // CZCOLORUTILS_H
