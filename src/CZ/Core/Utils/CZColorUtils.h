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

    static constexpr Float32 U8ToFloat32(UInt8 x) noexcept { return x * (1.0f / 255.0f); }

    static constexpr UInt8 Float32ToU8(Float32 x) noexcept { return (UInt8)std::clamp(x * 255.0f + 0.5f, 0.0f, 255.0f); }

    static constexpr SkColor SrcOver(SkColor A, SkColor B) noexcept
    {
        // Extract unpremultiplied 0–255 channels
        Float32 Ar = U8ToFloat32(SkColorGetR(A));
        Float32 Ag = U8ToFloat32(SkColorGetG(A));
        Float32 Ab = U8ToFloat32(SkColorGetB(A));
        Float32 Aa = U8ToFloat32(SkColorGetA(A));

        Float32 Br = U8ToFloat32(SkColorGetR(B));
        Float32 Bg = U8ToFloat32(SkColorGetG(B));
        Float32 Bb = U8ToFloat32(SkColorGetB(B));
        Float32 Ba = U8ToFloat32(SkColorGetA(B));

        // Convert to premultiplied
        Float32 Apr = Ar * Aa;
        Float32 Apg = Ag * Aa;
        Float32 Apb = Ab * Aa;

        Float32 Bpr = Br * Ba;
        Float32 Bpg = Bg * Ba;
        Float32 Bpb = Bb * Ba;

        // ---- kSrcOver : B over A ----
        Float32 outA  = Ba + Aa * (1.0f - Ba);
        Float32 outPr = Bpr + Apr * (1.0f - Ba);
        Float32 outPg = Bpg + Apg * (1.0f - Ba);
        Float32 outPb = Bpb + Apb * (1.0f - Ba);

        // Convert back to unpremultiplied
        Float32 invA = (outA > 0.0f) ? (1.0f / outA) : 0.0f;

        UInt8 r = Float32ToU8(outPr * invA);
        UInt8 g = Float32ToU8(outPg * invA);
        UInt8 b = Float32ToU8(outPb * invA);
        UInt8 a = Float32ToU8(outA);

        return SkColorSetARGB(a, r, g, b);
    }

    static constexpr SkColor StateSrcOver(SkColor A, SkColor B, Float32 opacity) noexcept
    {
        return SrcOver(A, SkColorSetA(B, SkColorGetA(B) * std::clamp(opacity, 0.f, 1.f)));
    }
};

#endif // CZCOLORUTILS_H
