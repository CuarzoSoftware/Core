#ifndef CZ_CZPRESENTATIONTIME_H
#define CZ_CZPRESENTATIONTIME_H

#include <CZ/Core/CZBitset.h>
#include <ctime>

/**
 * @brief Presentation time information
 */
struct CZ::CZPresentationTime
{
    /**
     * @brief Flags indicating how the presentation was done
     */
    enum Flags
    {
        VSync           = 0x1, ///< Presentation was vsync'd.
        HWClock         = 0x2, ///< Hardware provided the presentation timestamp.
        HWCompletion    = 0x4, ///< Hardware signaled the start of the presentation.
    };

    UInt64 paintEventId;    /// Id of the paint event

    timespec time;          ///< The presentation timestamp.
    UInt32 period;          ///< Nanoseconds prediction until the next refresh. Zero if unknown.
    UInt64 seq;             ///< Vertical retrace counter. Zero if unknown.
    CZBitset<Flags> flags;  ///< Flags indicating how the presentation was done.
};

#endif // CZ_CZPRESENTATIONTIME_H
