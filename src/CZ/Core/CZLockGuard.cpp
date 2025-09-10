#include <CZ/Core/CZLockGuard.h>
#include <mutex>

using namespace CZ;

static std::mutex Mutex;
thread_local bool DidThreadLock { false };

bool CZLockGuard::Lock() noexcept
{
    if (DidThreadLock)
        return false;

    DidThreadLock = true;
    Mutex.lock();
    return true;
}

bool CZLockGuard::Unlock() noexcept
{
    if (!DidThreadLock)
        return false;

    DidThreadLock = false;
    Mutex.unlock();
    return true;
}
