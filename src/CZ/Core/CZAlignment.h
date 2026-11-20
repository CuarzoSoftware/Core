#ifndef CZ_CZALIGNMENT_H
#define CZ_CZALIGNMENT_H

#include <CZ/Core/CZEdge.h>
#include <string_view>

namespace CZ
{
    /**
     * @brief Alignment within a container, expressed as the CZEdge flags it sticks to.
     *
     * No edges means centered; one edge aligns to that side; two adjacent edges align to a corner.
     */
    enum CZAlignment
    {
        CZAlignCenter       = CZEdgeNone,   ///< Centered.
        CZAlignLeft         = CZEdgeLeft,   ///< Left side, vertically centered.
        CZAlignTop          = CZEdgeTop,    ///< Top side, horizontally centered.
        CZAlignRight        = CZEdgeRight,  ///< Right side, vertically centered.
        CZAlignBottom       = CZEdgeBottom, ///< Bottom side, horizontally centered.

        CZAlignTopLeft      = CZEdgeTop | CZEdgeLeft,       ///< Top-left corner.
        CZAlignTopRight     = CZEdgeTop | CZEdgeRight,      ///< Top-right corner.
        CZAlignBottomRight  = CZEdgeBottom | CZEdgeRight,   ///< Bottom-right corner.
        CZAlignBottomLeft   = CZEdgeBottom | CZEdgeLeft     ///< Bottom-left corner.
    };

    /**
     * @brief Converts a set of edges into a CZAlignment.
     *
     * All four edges map to CZAlignCenter, and three edges map to the side shared by them
     * (e.g. left, top and right map to CZAlignTop). Other combinations are cast directly.
     */
    static inline constexpr CZAlignment AlignmentFromEdges(CZBitset<CZEdge> edges) noexcept
    {
        if (edges.get() == (CZEdgeLeft | CZEdgeTop | CZEdgeRight | CZEdgeBottom))
            return CZAlignCenter;
        else if (edges.get() == (CZEdgeLeft | CZEdgeTop | CZEdgeRight))
            return CZAlignTop;
        else if (edges.get() == (CZEdgeBottom | CZEdgeTop | CZEdgeRight))
            return CZAlignRight;
        else if (edges.get() == (CZEdgeBottom | CZEdgeLeft | CZEdgeRight))
            return CZAlignBottom;
        else if (edges.get() == (CZEdgeBottom | CZEdgeLeft | CZEdgeTop))
            return CZAlignLeft;
        return static_cast<CZAlignment>(edges.get());
    }

    /** @brief Human-readable name of @p alignment (e.g. "Top Left"), or "Unknown". */
    static inline constexpr std::string_view AlignmentString(CZAlignment alignment) noexcept
    {
        switch (alignment)
        {
        case CZAlignCenter:
            return "Center";
        case CZAlignLeft:
            return "Left";
        case CZAlignTop:
            return "Top";
        case CZAlignRight:
            return "Right";
        case CZAlignBottom:
            return "Bottom";
        case CZAlignTopLeft:
            return "Top Left";
        case CZAlignTopRight:
            return "Top Right";
        case CZAlignBottomRight:
            return "Bottom Right";
        case CZAlignBottomLeft:
            return "Bottom Left";
        default:
            return "Unknown";
        }
    }
}

#endif // CZ_CZALIGNMENT_H
