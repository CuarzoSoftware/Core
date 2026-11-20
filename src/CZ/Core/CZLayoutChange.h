#ifndef CZ_CZLAYOUTCHANGE_H
#define CZ_CZLAYOUTCHANGE_H

namespace CZ
{
    /** @brief Flags describing which layout properties changed. */
    enum CZLayoutChange
    {
        CZLayoutChangePos   = 1 << 0, ///< The position changed.
        CZLayoutChangeSize  = 1 << 1, ///< The size changed.
        CZLayoutChangeScale = 1 << 2  ///< The scale factor changed.
    };
}

#endif // CZ_CZLAYOUTCHANGE_H
