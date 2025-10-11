#include <CZ/Core/CZSafeEventQueue.h>
#include <CZ/Core/Events/CZEvent.h>
#include <CZ/Core/CZCore.h>
#include <CZ/Core/CZLog.h>

using namespace CZ;

void CZSafeEventQueue::addEvent(std::shared_ptr<CZEvent> event, CZObject &object) noexcept
{
    if (!event) return;
    m_queue.emplace(CZWeak<CZObject>(&object), event);
}

void CZSafeEventQueue::dispatch() noexcept
{
    auto core { CZCore::Get() };

    if (core.use_count() == 1)
    {
        CZLog(CZError, CZLN, "Missing CZCore");
        m_queue = {};
        return;
    }

    while (!m_queue.empty())
    {
        if (m_queue.front().object)
            core->sendEvent(*m_queue.front().event, *m_queue.front().object.get());
        m_queue.pop();
    }
}
