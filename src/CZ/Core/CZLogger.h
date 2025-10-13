#ifndef CZLOGGER_H
#define CZLOGGER_H

#include <CZ/Core/Cuarzo.h>
#include <iostream>
#include <algorithm>
#include <format>
#include <memory>
#include <source_location>
#include <string>
#include <array>
#define CZLN std::source_location::current()

namespace CZ
{
    enum CZLogLevel : UInt32
    {
        CZSilent = 0u,
        CZFatal,
        CZError,
        CZWarning,
        CZInfo,
        CZDebug,
        CZTrace
    };
}

class CZ::CZLogger
{
public:
    CZLogger(const std::string &ns = "", const char *env = nullptr) noexcept;

    const std::string &ns() const noexcept { return m_ns; }

    const std::string &ctx() const noexcept { return m_ctx; }

    CZLogLevel level() const noexcept { return *m_lvl; }

    void setLevel(CZLogLevel level) const noexcept
    {
        *m_lvl = std::clamp<CZLogLevel>(level, CZSilent, CZTrace);
    }

    /// Creates a new logger with this context + the appended context (the level is shared)
    [[nodiscard]] CZLogger newWithContext(const std::string &context) const noexcept
    {
        CZLogger newLogger(*this);
        newLogger.m_ctx += "[" + context + "] ";
        return newLogger;
    }

    template<typename... Args>
    constexpr void operator()(CZLogLevel level, std::format_string<Args...> fmt, Args&&... args) const
    {
        if (level > *m_lvl)
            return;

        if (level < CZWarning)
            std::cerr << LogLevelColor[level] << m_ns << " " << LogLevelString[level] << "\033[0m: " << m_ctx << std::format(fmt, std::forward<Args>(args)...) << "\n";
        else if (level <= CZTrace)
            std::cout << LogLevelColor[level] << m_ns << " " << LogLevelString[level] << "\033[0m: " << m_ctx << std::format(fmt, std::forward<Args>(args)...) << "\n";
    }

    template<typename... Args>
    constexpr void operator()(CZLogLevel level, std::source_location source, std::format_string<Args...> fmt, Args&&... args) const
    {
        if (level > *m_lvl)
            return;

        if (level < CZWarning)
            std::cerr << LogLevelColor[level] << m_ns << " " << LogLevelString[level] << "\033[0m: " << m_ctx << std::format(fmt, std::forward<Args>(args)...) << " "
                      << source.file_name() << "(" << source.line() << ":" << source.column() << ")\n";
        else if (level <= CZTrace)
            std::cout << LogLevelColor[level] << m_ns << " " << LogLevelString[level] << "\033[0m: " << m_ctx << std::format(fmt, std::forward<Args>(args)...) << " "
                      << source.file_name() << "(" << source.line() << ":" << source.column() << ")\n";
    }

private:
    static inline constexpr std::array<std::string_view, 7> LogLevelString
    {
        "Silent",
        "Fatal",
        "Error",
        "Warning",
        "Info",
        "Debug",
        "Trace"
    };

    static inline constexpr std::array<std::string_view, 7> LogLevelColor
    {
        "",
        "\033[91m", // Fatal   - Bright Red
        "\033[31m", // Error   - Red
        "\033[33m", // Warning - Yellow
        "\033[32m", // Info    - Green
        "\033[36m", // Debug   - Cyan
        "\033[37m"  // Trace   - Light Gray
    };
    std::shared_ptr<CZLogLevel> m_lvl { new CZLogLevel() };
    std::string m_ns;
    std::string m_ctx;
};

#endif // CZLOGGER_H
