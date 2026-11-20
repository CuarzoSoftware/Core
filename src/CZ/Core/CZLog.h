#ifndef CZ_CZLOG_H
#define CZ_CZLOG_H

#include <CZ/Core/CZLogger.h>

/** @brief The CZCore logger (see CZCoreLoggerGet()). */
#define CZLog CZCoreLoggerGet()

/** @brief Logger used by CZCore, with namespace "Cuarzo" and level read from `CZ_CORE_LOG_LEVEL`. */
const CZ::CZLogger &CZCoreLoggerGet() noexcept;

#endif // CZ_CZLOG_H
