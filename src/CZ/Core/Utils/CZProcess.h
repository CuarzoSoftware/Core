#ifndef CZPROCESS_H
#define CZPROCESS_H

#include <CZ/Core/Cuarzo.h>
#include <string>
#include <vector>

/**
 * @brief Launching processes without blocking the caller.
 */
class CZ::CZProcess
{
public:
    /**
     * @brief Starts a detached process (an app, a helper command, ...) without blocking.
     *
     * It's spawned from a background thread (vfork-like posix_spawn, never a fork() of the
     * caller, which can take long for processes with large GPU mappings), in its own session,
     * reparented to init (never the caller's zombie), with default signal handlers and mask and
     * only the standard file descriptors inherited. Its output goes where the caller's does.
     *
     * @param argv The program (searched in PATH) and its arguments.
     * @return false if @p argv is empty, otherwise true (whether it actually started isn't known).
     */
    static bool SpawnDetached(std::vector<std::string> argv) noexcept;
};

#endif // CZPROCESS_H
