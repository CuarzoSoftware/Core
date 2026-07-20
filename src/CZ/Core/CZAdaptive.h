#ifndef CZ_CZADAPTIVE_H
#define CZ_CZADAPTIVE_H

#include <CZ/Core/Cuarzo.h>
#include <CZ/Core/CZColorScheme.h>

/**
 * @brief A value with distinct light and dark appearances.
 *
 * Holds two values of type @p T and resolves to one of them based on a CZColorScheme, mirroring
 * macOS / SwiftUI dynamic system colors. Trivial and constexpr (when @p T is), so it can be used
 * as a compile-time constant (e.g. the named colors in AKTheme).
 *
 * @see CZAdaptiveColor, the CZAdaptive<SkColor> alias (declared in Cuarzo.h).
 */
template<class T>
class CZ::CZAdaptive
{
public:
    /** @brief Separate light and dark variants. */
    constexpr CZAdaptive(T light, T dark) noexcept :
        m_light(light), m_dark(dark) {}

    /** @brief The same value in both appearances. */
    constexpr CZAdaptive(T both = T{}) noexcept :
        m_light(both), m_dark(both) {}

    constexpr const T &light() const noexcept { return m_light; }
    constexpr const T &dark() const noexcept { return m_dark; }

    /** @brief The variant for @p scheme: dark for CZColorScheme::Dark, light otherwise (Light/Unknown). */
    constexpr const T &resolve(CZColorScheme scheme) const noexcept
    {
        return scheme == CZColorScheme::Dark ? m_dark : m_light;
    }

    constexpr bool operator==(const CZAdaptive &other) const noexcept
    {
        return m_light == other.m_light && m_dark == other.m_dark;
    }
    constexpr bool operator!=(const CZAdaptive &other) const noexcept { return !(*this == other); }

private:
    T m_light;
    T m_dark;
};

#endif // CZ_CZADAPTIVE_H
