#ifndef CZ_CZLAYOUTCHANGE_H
#define CZ_CZLAYOUTCHANGE_H

namespace CZ
{
    enum CZLayoutChange
    {
        CZLayoutChangePos   = 1 << 0,
        CZLayoutChangeSize  = 1 << 1,
        CZLayoutChangeScale = 1 << 2
    };
}

#endif // CZ_CZLAYOUTCHANGE_H
