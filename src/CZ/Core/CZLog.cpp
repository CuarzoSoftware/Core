#include <CZ/Core/CZLog.h>

using namespace CZ;

const CZ::CZLogger &CZCoreLoggerGet() noexcept
{
    static CZLogger logger { "Cuarzo", "CZ_CORE_LOG_LEVEL" };
    return logger;
}
