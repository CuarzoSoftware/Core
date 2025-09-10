#ifndef CZ_CZLOCKGUARD_H
#define CZ_CZLOCKGUARD_H

namespace CZ
{
    // Do not use it
    struct CZLockGuard
    {
        // Creates a scoped lock
        CZLockGuard() noexcept : didLock(CZLockGuard::Lock()) {}

        ~CZLockGuard() noexcept
        {
            if (didLock)
                CZLockGuard::Unlock();
        }

        // true if locked, false if already locked
        static bool Lock() noexcept;

        // true if unlocked, false if already unlocked
        static bool Unlock() noexcept;
    private:
        bool didLock;
    };
}

#endif // CZ_CZLOCKGUARD_H
