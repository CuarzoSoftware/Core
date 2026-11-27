#include <CZ/Core/CZMouseSettings.h>
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <mutex>
#include <string>
#include <sys/stat.h>

using namespace CZ;

static std::string FilePath() noexcept
{
    if (const char *config { getenv("XDG_CONFIG_HOME") }; config && config[0] == '/')
        return std::string(config) + "/cz-crystals/mouse.conf";

    if (const char *home { getenv("HOME") })
        return std::string(home) + "/.config/cz-crystals/mouse.conf";

    return {};
}

UInt32 CZMouseSettings::DoubleClickMs() noexcept
{
    static std::mutex mutex;
    static const std::string path { FilePath() };
    static struct timespec mtime { 0, -1 }; // Of the file as last read
    static UInt32 value { DefaultDoubleClickMs };

    std::lock_guard lock { mutex };
    struct stat st {};

    if (path.empty() || stat(path.c_str(), &st) != 0)
    {
        mtime = { 0, -1 };
        return value = DefaultDoubleClickMs;
    }

    if (st.st_mtim.tv_sec == mtime.tv_sec && st.st_mtim.tv_nsec == mtime.tv_nsec)
        return value;

    mtime = st.st_mtim;
    value = DefaultDoubleClickMs;
    std::ifstream file { path };
    std::string line;

    while (std::getline(file, line))
        if (line.rfind("DoubleClickMs=", 0) == 0)
            value = UInt32(std::clamp(std::atoi(line.c_str() + 14), 100, 5000));

    return value;
}
