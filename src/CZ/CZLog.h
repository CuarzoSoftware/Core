#ifndef CZ_CZLOG_H
#define CZ_CZLOG_H

#include <CZLogger.h>

#define CZLog CZLogGet()

const CZ::CZLogger &CZLogGet() noexcept;

#endif // CZ_CZLOG_H
