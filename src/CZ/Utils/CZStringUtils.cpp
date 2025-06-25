#include <CZ/Utils/CZStringUtils.h>
#include <cstring>

using namespace CZ;

bool CZStringUtils::CheckExtension(const char *extensions, const char *extension) noexcept
{
    if (!extensions || !extension)
        return false;

    size_t extlen = strlen(extension);
    const char *end = extensions + strlen(extensions);

    while (extensions < end)
    {
        if (*extensions == ' ')
        {
            extensions++;
            continue;
        }

        size_t n = strcspn(extensions, " ");

        if (n == extlen && strncmp(extension, extensions, n) == 0)
            return true;

        extensions += n;
    }

    return false;
}
