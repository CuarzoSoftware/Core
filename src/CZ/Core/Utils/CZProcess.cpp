#include <CZ/Core/Utils/CZProcess.h>
#include <condition_variable>
#include <deque>
#include <mutex>
#include <thread>
#include <csignal>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>

extern char **environ;

using namespace CZ;

namespace
{
    // Never destroyed (the thread may still be waiting when the process exits)
    struct Spawner
    {
        std::mutex mutex;
        std::condition_variable cv;
        std::deque<std::vector<std::string>> queue;
    };

    Spawner *s_spawner {};
    std::once_flag s_once;

    // Returns the pid, or -1
    pid_t Spawn(const std::vector<std::string> &args) noexcept
    {
        std::vector<char*> argv;
        argv.reserve(args.size() + 1);

        for (const auto &a : args)
            argv.push_back(const_cast<char*>(a.c_str()));

        argv.push_back(nullptr);

        posix_spawnattr_t attr;
        posix_spawnattr_init(&attr);
        posix_spawnattr_setflags(&attr, POSIX_SPAWN_SETSID | POSIX_SPAWN_SETSIGMASK | POSIX_SPAWN_SETSIGDEF);

        sigset_t none, all;
        sigemptyset(&none);
        sigfillset(&all);
        posix_spawnattr_setsigmask(&attr, &none);
        posix_spawnattr_setsigdefault(&attr, &all);

        posix_spawn_file_actions_t actions;
        posix_spawn_file_actions_init(&actions);
        posix_spawn_file_actions_addclosefrom_np(&actions, 3);

        pid_t pid;
        const int ret { posix_spawnp(&pid, argv[0], &actions, &attr, argv.data(), environ) };
        posix_spawn_file_actions_destroy(&actions);
        posix_spawnattr_destroy(&attr);
        return ret == 0 ? pid : -1;
    }

    void Run(std::vector<std::string> args) noexcept
    {
        // Through setsid -f (util-linux): it forks the program and exits right away, so the program is
        // reparented to init and only setsid has to be reaped here.
        std::vector<std::string> viaSetsid { "setsid", "-f" };
        viaSetsid.insert(viaSetsid.end(), args.begin(), args.end());

        if (const pid_t pid { Spawn(viaSetsid) }; pid > 0)
        {
            waitpid(pid, nullptr, 0);
            return;
        }

        // No setsid: started directly, reaped by its own thread once it exits
        if (const pid_t pid { Spawn(args) }; pid > 0)
            std::thread([pid]{ waitpid(pid, nullptr, 0); }).detach();
    }

    void Loop() noexcept
    {
        while (true)
        {
            std::vector<std::string> args;

            {
                std::unique_lock lock { s_spawner->mutex };
                s_spawner->cv.wait(lock, []{ return !s_spawner->queue.empty(); });
                args = std::move(s_spawner->queue.front());
                s_spawner->queue.pop_front();
            }

            Run(std::move(args));
        }
    }
}

bool CZProcess::SpawnDetached(std::vector<std::string> argv) noexcept
{
    if (argv.empty() || argv[0].empty())
        return false;

    std::call_once(s_once, []{
        s_spawner = new Spawner();
        std::thread(Loop).detach();
    });

    {
        std::lock_guard lock { s_spawner->mutex };
        s_spawner->queue.emplace_back(std::move(argv));
    }

    s_spawner->cv.notify_one();
    return true;
}
