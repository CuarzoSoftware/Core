#include <CZ/CZBooleanEventSource.h>
#include <CZ/CZCore.h>
#include <CZ/CZLog.h>
#include <sys/eventfd.h>

using namespace CZ;

std::shared_ptr<CZBooleanEventSource> CZBooleanEventSource::Make(bool enabled, const Callback &callback) noexcept
{
    auto core { CZCore::Get() };

    if (core.use_count() == 1)
    {
        CZLog(CZError, CZLN, "Missing CZCore");
        return {};
    }

    const int fd { eventfd(enabled ? 1 : 0, EFD_CLOEXEC) };

    if (fd < 0)
    {
        CZLog(CZError, CZLN, "eventfd failed");
        return {};
    }

    auto instance { std::shared_ptr<CZBooleanEventSource>(new CZBooleanEventSource(enabled, callback)) };
    auto *ptr { instance.get() };

    instance->m_source = CZEventSource::Make(fd, EPOLLIN, [ptr](int, UInt32) {
        ptr->setState(false);
        if (ptr->m_callback)
            ptr->m_callback(ptr);
    });

    if (!instance->m_source)
    {
        CZLog(CZError, CZLN, "Failed to create CZEventSource");
        close(fd);
        return {};
    }

    return instance;
}

void CZBooleanEventSource::setState(bool enabled) noexcept
{
    if (m_state == enabled)
        return;

    m_state = enabled;

    if (m_state)
        eventfd_write(m_source->fd(), 1);
    else
    {
        eventfd_t value;
        eventfd_read(m_source->fd(), &value);
    }
}

CZBooleanEventSource::~CZBooleanEventSource() noexcept
{
    if (m_source)
        close(m_source->fd());
}


