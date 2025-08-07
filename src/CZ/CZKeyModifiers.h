#ifndef CZ_CZKEYMODIFIERS_H
#define CZ_CZKEYMODIFIERS_H

#include <CZ/Cuarzo.h>

namespace CZ
{
    /**
     * @brief Keyboard modifiers.
     */
    struct CZKeyModifiers
    {
        /// Active modifiers when physically pressed
        UInt32 depressed = 0;

        /// Hooked modifiers that will be disabled after a non-modifier key is pressed
        UInt32 latched = 0;

        /// Active modifiers until they are pressed again (e.g. the Shift key)
        UInt32 locked = 0;

        /// Group the above states (use this value if the source of a modifier change is not of your interest)
        UInt32 group = 0;
    };

    inline bool operator==(const CZKeyModifiers &a, const CZKeyModifiers &b)
    {
        return a.depressed == b.depressed &&
               a.latched   == b.latched &&
               a.locked    == b.locked &&
               a.group     == b.group;
    }

    inline bool operator!=(const CZKeyModifiers &a, const CZKeyModifiers &b)
    {
        return !(a == b);
    }
};

#endif // CZ_CZKEYMODIFIERS_H
