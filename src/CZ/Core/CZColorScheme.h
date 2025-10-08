#ifndef CZ_CZCOLORSCHEME_H
#define CZ_CZCOLORSCHEME_H

namespace CZ
{
    /**
     * Represents the color scheme preference, matching the 'color-scheme' property
     * from the XDG Settings portal.
     */
    enum class CZColorScheme
    {
        /**
         * The color scheme is unknown or not specified.
         *
         * All Cuarzo objects will fall back to the Light scheme when this
         * value is used.
         */
        Unknown,

        /**
         * Dark color scheme preference.
         */
        Dark,

        /**
         * Light color scheme preference.
         */
        Light
    };
};

#endif // CZ_CZCOLORSCHEME_H
