#ifndef CZSTRINGUTILS_H
#define CZSTRINGUTILS_H

#include <CZ/CZ.h>

class CZ::CZStringUtils
{
public:
    static bool CheckExtension(const char *extensions, const char *extension) noexcept;
};

#endif // CZSTRINGUTILS_H
