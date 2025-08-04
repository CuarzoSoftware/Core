#include <CZ/CZLog.h>

using namespace CZ;

const CZ::CZLogger &CZLogGet() noexcept
{
    static CZLogger logger { "Cuarzo", "CZ_LOG_LEVEL" };
    return logger;
}
