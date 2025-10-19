#ifndef CZ_CZALIGNMENT_H
#define CZ_CZALIGNMENT_H

#include <CZ/Core/CZEdge.h>
#include <string_view>

namespace CZ
{
    enum CZAlignment
    {
        CZAlignCenter       = CZEdgeNone,
        CZAlignLeft         = CZEdgeLeft,
        CZAlignTop          = CZEdgeTop,
        CZAlignRight        = CZEdgeRight,
        CZAlignBottom       = CZEdgeBottom,

        CZAlignTopLeft      = CZEdgeTop | CZEdgeLeft,
        CZAlignTopRight     = CZEdgeTop | CZEdgeRight,
        CZAlignBottomRight  = CZEdgeBottom | CZEdgeRight,
        CZAlignBottomLeft   = CZEdgeBottom | CZEdgeLeft
    };

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
