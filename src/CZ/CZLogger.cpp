#include <CZ/CZLogger.h>

using namespace CZ;

CZLogger::CZLogger(const std::string &ns, const char *env) noexcept : m_ns{ns}
{
    if (env)
    {
        const char *envLevel { getenv(env) };

        if (envLevel)
            setLevel(static_cast<CZLogLevel>(atoi(envLevel)));
    }
}

