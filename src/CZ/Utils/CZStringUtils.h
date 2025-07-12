#ifndef CZSTRINGUTILS_H
#define CZSTRINGUTILS_H

#include <CZ/CZ.h>
#include <string>

class CZ::CZStringUtils
{
public:
    static bool CheckExtension(const char *extensions, const char *extension) noexcept;

    static constexpr std::string SubStrAfterLastOf(const std::string &string, const std::string &sub)
    {
        const size_t pos { string.find_last_of(sub) };
        if (pos == std::string::npos)
            return {};

        return string.substr(pos + 1);
    }
};

#endif // CZSTRINGUTILS_H
