#include <CZ/CZSafeEventQueue.h>
#include <CZ/Events/CZEvent.h>
#include <CZ/CZCore.h>

using namespace CZ;

void CZSafeEventQueue::addEvent(const CZEvent &event, CZObject &object) noexcept
{
    m_queue.emplace(CZWeak<CZObject>(&object), std::unique_ptr<CZEvent>(event.copy()));
}

void CZSafeEventQueue::dispatch() noexcept
{
    while (!m_queue.empty())
    {
        if (m_queue.front().object)
            akApp()->sendEvent(*m_queue.front().event, *m_queue.front().object.get());
        m_queue.pop();
    }
}
