#ifndef CZBUSWORKER_H
#define CZBUSWORKER_H

#include <CZ/Core/CZObject.h>
#include <condition_variable>
#include <deque>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

struct sd_bus;

/**
 * @brief Runs blocking D-Bus work off the main thread.
 *
 * A single background thread runs the jobs in order, each given that thread's own bus connection
 * (sd-bus connections can't be shared between threads). A job returns what to do next on the main
 * thread (e.g. apply the values it read), run from the CZCore event loop.
 *
 * @code
 * CZBusWorker worker { CZBusWorker::System };
 *
 * worker.run([](sd_bus *bus) -> std::function<void()> {
 *     int enabled {};
 *     sd_bus_get_property_trivial(bus, "org.freedesktop.NetworkManager", ..., 'b', &enabled);
 *     return [enabled]{ updateUI(enabled); }; // Main thread
 * });
 * @endcode
 *
 * Jobs run on another thread: they must not touch the owner's state (copy what they need), their
 * completions may. Destroying the worker waits for the running job and drops the pending ones and
 * their completions.
 */
class CZ::CZBusWorker final : public CZObject
{
public:
    /// The bus the jobs get.
    enum Bus
    {
        System, ///< The system bus
        User    ///< The user (session) bus
    };

    /**
     * @brief The work (worker thread; @p bus is nullptr if it couldn't be opened), returning its
     *        completion (main thread, may be empty).
     */
    using Job = std::function<std::function<void()>(sd_bus *bus)>;

    /**
     * @brief Starts the thread, which opens a connection to @p bus.
     *
     * @param timeoutMs Method calls never wait longer (a busy service must not block the queue).
     */
    explicit CZBusWorker(Bus bus = System, UInt32 timeoutMs = 5000) noexcept;
    ~CZBusWorker() noexcept;

    /// Queues @p job (run right away, here, if the worker couldn't start).
    void run(Job job) noexcept;

private:
    void loop() noexcept;
    void dispatch() noexcept;

    Bus m_bus;
    UInt32 m_timeoutMs;
    std::mutex m_mutex;
    std::condition_variable m_cv;
    std::deque<Job> m_jobs;
    std::vector<std::function<void()>> m_done;
    bool m_quit { false };
    std::shared_ptr<CZEventSource> m_wake; // eventfd: completions ready
    std::thread m_thread;
};

#endif // CZBUSWORKER_H
