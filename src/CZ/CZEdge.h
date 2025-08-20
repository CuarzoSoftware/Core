#ifndef CZ_CZEDGE_H
#define CZ_CZEDGE_H

#include <CZ/CZBitset.h>
#include <limits>

namespace CZ
{
    /**
     * @brief Edge flags.
     */
    enum CZEdge : UInt32
    {
        CZEdgeNone   = 0, ///< No edge.
        CZEdgeTop    = static_cast<UInt32>(1) << 0, ///< The top edge.
        CZEdgeBottom = static_cast<UInt32>(1) << 1, ///< The bottom edge.
        CZEdgeLeft   = static_cast<UInt32>(1) << 2, ///< The left edge.
        CZEdgeRight  = static_cast<UInt32>(1) << 3, ///< The right edge.
    };

    /**
     * @brief Represents a disabled edge.
     *
     * This constant indicates that an edge is disabled, for example, in LToplevelRole constraints.
     */
    static inline constexpr Int32 CZEdgeDisabled = std::numeric_limits<Int32>::min();

    /**
     * @brief Checks if the given edges form a corner by being orthogonal.
     *
     * This function verifies if the provided bitset of edges corresponds to one of the four possible
     * corner configurations: (Top-Left, Top-Right, Bottom-Left, Bottom-Right).
     *
     * @param edges A bitset representing the edges.
     * @return `true` if the edges form a corner, `false` otherwise.
     */
    inline constexpr bool EdgeIsCorner(CZBitset<CZEdge> edges) noexcept
    {
        return edges == (CZEdgeTop | CZEdgeLeft)
            || edges == (CZEdgeTop | CZEdgeRight)
            || edges == (CZEdgeBottom | CZEdgeLeft)
            || edges == (CZEdgeBottom | CZEdgeRight);
    }
};

#endif // CZ_CZEDGE_H
