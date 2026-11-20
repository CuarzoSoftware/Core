#ifndef CZ_CZWINDOWSTATE_H
#define CZ_CZWINDOWSTATE_H

namespace CZ
{
    /** @brief Window state flags (they can be combined). */
    enum CZWindowState
    {
        CZWinNoState     = 0,       ///< No state.
        CZWinMaximized   = 1 << 1,  ///< Maximized.
        CZWinFullscreen  = 1 << 2,  ///< Fullscreen.
        CZWinResizing    = 1 << 3,  ///< Being interactively resized.
        CZWinActivated   = 1 << 4,  ///< Active (has focus).
        CZWinTiledLeft   = 1 << 5,  ///< Left edge tiled against something.
        CZWinTiledRight  = 1 << 6,  ///< Right edge tiled against something.
        CZWinTiledTop    = 1 << 7,  ///< Top edge tiled against something.
        CZWinTiledBottom = 1 << 8,  ///< Bottom edge tiled against something.
        CZWinSuspended   = 1 << 9,  ///< Suspended (e.g. not visible, so it shouldn't render).
        /// All of the above states.
        CZWinAllWindowStates = CZWinMaximized | CZWinFullscreen | CZWinResizing | CZWinActivated |
                               CZWinTiledLeft | CZWinTiledRight | CZWinTiledTop | CZWinTiledBottom | CZWinSuspended
    };
}

#endif // CZ_CZWINDOWSTATE_H
