#ifndef CZ_CZLOG_H
#define CZ_CZLOG_H

#include <CZ/Core/CZLogger.h>

#define CZLog CZCoreLoggerGet()

const CZ::CZLogger &CZCoreLoggerGet() noexcept;

#endif // CZ_CZLOG_H
