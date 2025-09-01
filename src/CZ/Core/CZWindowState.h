#ifndef CZ_CZWINDOWSTATE_H
#define CZ_CZWINDOWSTATE_H

namespace CZ
{
    enum CZWindowState
    {
        CZWinNoState     = 0,
        CZWinMaximized   = 1 << 1,
        CZWinFullscreen  = 1 << 2,
        CZWinResizing    = 1 << 3,
        CZWinActivated   = 1 << 4,
        CZWinTiledLeft   = 1 << 5,
        CZWinTiledRight  = 1 << 6,
        CZWinTiledTop    = 1 << 7,
        CZWinTiledBottom = 1 << 8,
        CZWinSuspended   = 1 << 9,
        CZWinAllWindowStates = CZWinMaximized | CZWinFullscreen | CZWinResizing | CZWinActivated |
                               CZWinTiledLeft | CZWinTiledRight | CZWinTiledTop | CZWinTiledBottom | CZWinSuspended
    };
}

#endif // CZ_CZWINDOWSTATE_H
