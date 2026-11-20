#ifndef CZSTRINGUTILS_H
#define CZSTRINGUTILS_H

#include <CZ/Core/Cuarzo.h>
#include <string>

/**
 * @brief String helper functions.
 */
class CZ::CZStringUtils
{
public:
    /**
     * @brief Checks if @p extension appears in a space-separated list of extension names
     *        (e.g. the EGL/GL extension strings).
     *
     * @return `false` if not found or if any argument is `nullptr`.
     */
    static bool CheckExtension(const char *extensions, const char *extension) noexcept;

    /**
     * @brief Returns the part of @p string after the last occurrence of any character in @p sub.
     *
     * @return An empty string if no character of @p sub is found.
     */
    static constexpr std::string SubStrAfterLastOf(const std::string &string, const std::string &sub)
    {
        const size_t pos { string.find_last_of(sub) };
        if (pos == std::string::npos)
            return {};

        return string.substr(pos + 1);
    }
};

#endif // CZSTRINGUTILS_H
