#include <CZ/Core/CZTime.h>

using namespace CZ;

static UInt32 s_serial { 0 };

UInt32 CZTime::NextSerial() noexcept
{
    s_serial++;
    if (s_serial == 0) s_serial++;
    return s_serial;
}






