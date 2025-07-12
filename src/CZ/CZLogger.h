#ifndef CZLOGGER_H
#define CZLOGGER_H

#include <CZ/CZ.h>
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
        CZFatal = 1u,
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
    CZLogger(const std::string &ns = "") noexcept : m_ns{ns} {}

    const std::string &ns() const noexcept { return m_ns; }
    const std::string &ctx() const noexcept { return m_ctx; }

    void setLevel(CZLogLevel level) const noexcept
    {
        *m_lvl = std::clamp<UInt32>(level, 0, CZTrace);
    }

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
                      << source.function_name() << " " << source.file_name() << "(" << source.line() << ":" << source.column() << ")\n";
        else if (level <= CZTrace)
            std::cout << LogLevelColor[level] << m_ns << " " << LogLevelString[level] << "\033[0m: " << m_ctx << std::format(fmt, std::forward<Args>(args)...) << " "
                      << source.function_name() << " " << source.file_name() << "(" << source.line() << ":" << source.column() << ")\n";
    }

private:
    static inline constexpr std::array<std::string_view, 7> LogLevelString
    {
        "",
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
    std::shared_ptr<UInt32> m_lvl { new UInt32() };
    std::string m_ns;
    std::string m_ctx;
};

#endif // CZLOGGER_H
