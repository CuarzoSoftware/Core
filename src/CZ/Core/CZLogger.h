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
/** @brief Source location of the call site, to pass to CZLogger::operator()() so the file and line are printed. */
#define CZLN std::source_location::current()

namespace CZ
{
    /** @brief Log severity levels. A logger prints messages with a level less than or equal to its own. */
    enum CZLogLevel : UInt32
    {
        CZSilent = 0u,  ///< Nothing is printed.
        CZFatal,        ///< Unrecoverable errors.
        CZError,        ///< Errors.
        CZWarning,      ///< Warnings.
        CZInfo,         ///< Informational messages.
        CZDebug,        ///< Debugging messages.
        CZTrace         ///< Very verbose tracing messages.
    };
}

/**
 * @brief Leveled logger with `std::format` style formatting.
 *
 * Messages are printed as `<namespace> <Level>: <context><message>`, to `stderr` for CZFatal and
 * CZError and to `stdout` for the rest. Messages above the logger level are discarded.
 *
 * @code
 * CZLogger log { "MyApp", "MY_APP_LOG_LEVEL" };
 * log(CZInfo, "Loaded {} items", count);
 * log(CZError, CZLN, "Failed to open {}", path); // Also prints the file and line
 * @endcode
 */
class CZ::CZLogger
{
public:
    /**
     * @brief Creates a logger.
     *
     * The level is CZSilent unless @p env is given and that environment variable holds a level
     * number (0 to 6), e.g. `MY_APP_LOG_LEVEL=4` for CZInfo.
     *
     * @param ns Namespace printed before each message.
     * @param env Name of the environment variable to read the level from, or `nullptr`.
     */
    CZLogger(const std::string &ns = "", const char *env = nullptr) noexcept;

    /** @brief Namespace printed before each message. */
    const std::string &ns() const noexcept { return m_ns; }

    /** @brief Context prefix of the messages (see newWithContext()). */
    const std::string &ctx() const noexcept { return m_ctx; }

    /** @brief Current log level. */
    CZLogLevel level() const noexcept { return *m_lvl; }

    /** @brief Sets the log level (clamped to the valid range), shared with loggers created by newWithContext(). */
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

    /** @brief Logs a message formatted with `std::format` if @p level is enabled. */
    template<typename... Args>
    void operator()(CZLogLevel level, std::format_string<Args...> fmt, Args&&... args) const
    {
        if (level > *m_lvl)
            return;

        if (level < CZWarning)
            std::cerr << LogLevelColor[level] << m_ns << " " << LogLevelString[level] << "\033[0m: " << m_ctx << std::format(fmt, std::forward<Args>(args)...) << "\n";
        else if (level <= CZTrace)
            std::cout << LogLevelColor[level] << m_ns << " " << LogLevelString[level] << "\033[0m: " << m_ctx << std::format(fmt, std::forward<Args>(args)...) << "\n";
    }

    /** @brief Logs a message followed by the @p source location (usually CZLN) if @p level is enabled. */
    template<typename... Args>
    void operator()(CZLogLevel level, std::source_location source, std::format_string<Args...> fmt, Args&&... args) const
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
