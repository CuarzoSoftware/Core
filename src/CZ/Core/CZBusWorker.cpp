#include <CZ/Core/CZBusWorker.h>
#include <CZ/Core/CZEventSource.h>
#include <CZ/Core/CZLog.h>
#include <systemd/sd-bus.h>
#include <sys/eventfd.h>
#include <cstring>

using namespace CZ;

CZBusWorker::CZBusWorker(Bus bus, UInt32 timeoutMs) noexcept :
    m_bus(bus),
    m_timeoutMs(timeoutMs)
{
    const int fd { eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK) };

    if (fd < 0)
    {
        CZLog(CZError, CZLN, "eventfd failed: {}", strerror(errno));
        return;
    }

    m_wake = CZEventSource::Make(fd, EPOLLIN, CZOwn::Own, [this](int fd, UInt32, CZEventSource*) {
        eventfd_t v;
        eventfd_read(fd, &v);
        dispatch();
    });

    m_thread = std::thread([this]{ loop(); });
}

CZBusWorker::~CZBusWorker() noexcept
{
    {
        std::lock_guard lock { m_mutex };
        m_quit = true;
        m_jobs.clear();
    }

    m_cv.notify_one();

    if (m_thread.joinable())
        m_thread.join();
}

void CZBusWorker::run(Job job) noexcept
{
    if (!m_wake)
    {
        if (auto done { job(nullptr) })
            done();
        return;
    }

    {
        std::lock_guard lock { m_mutex };
        m_jobs.emplace_back(std::move(job));
    }

    m_cv.notify_one();
}

void CZBusWorker::loop() noexcept
{
    sd_bus *bus {};

    if ((m_bus == System ? sd_bus_open_system(&bus) : sd_bus_open_user(&bus)) < 0)
        bus = nullptr;
    else
        sd_bus_set_method_call_timeout(bus, UInt64(m_timeoutMs) * 1000);

    while (true)
    {
        Job job;

        {
            std::unique_lock lock { m_mutex };
            m_cv.wait(lock, [this]{ return m_quit || !m_jobs.empty(); });

            if (m_quit)
                break;

            job = std::move(m_jobs.front());
            m_jobs.pop_front();
        }

        auto done { job(bus) };

        if (!done)
            continue;

        {
            std::lock_guard lock { m_mutex };

            if (m_quit)
                break;

            m_done.emplace_back(std::move(done));
        }

        eventfd_write(m_wake->fd(), 1);
    }

    sd_bus_flush_close_unref(bus);
}

void CZBusWorker::dispatch() noexcept
{
    std::vector<std::function<void()>> done;

    {
        std::lock_guard lock { m_mutex };
        done.swap(m_done);
    }

    for (auto &f : done)
        f();
}
