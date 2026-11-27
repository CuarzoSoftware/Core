#ifndef CZ_CZMOUSESETTINGS_H
#define CZ_CZMOUSESETTINGS_H

#include <CZ/Core/Cuarzo.h>

/**
 * @brief The user's mouse preferences that apps honor themselves.
 *
 * Chosen in System Settings (Mouse) and stored in `$XDG_CONFIG_HOME/cz-crystals/mouse.conf`
 * (`~/.config/cz-crystals/mouse.conf` by default), next to the ones the compositor applies
 * (tracking, scrolling). The file is read again when it changes, so there's nothing to watch:
 * just ask each time (e.g. on every click).
 */
class CZ::CZMouseSettings
{
public:
    /// The default value of DoubleClickMs().
    static constexpr UInt32 DefaultDoubleClickMs { 400 };

    /// Longest time between the two presses of a double click, in milliseconds.
    static UInt32 DoubleClickMs() noexcept;
};

#endif // CZ_CZMOUSESETTINGS_H
