#ifndef CZ_CZLOCKGUARD_H
#define CZ_CZLOCKGUARD_H

namespace CZ
{
    // Do not use it
    /**
     * @brief Scoped lock of a single process-wide mutex.
     *
     * The lock is reentrant per thread: if the calling thread already holds it, constructing another
     * guard does nothing and its destructor doesn't unlock.
     *
     * @warning Not intended for public use.
     */
    class CZLockGuard
    {
    public:
        // Creates a scoped lock
        /** @brief Locks the mutex unless the calling thread already holds it. */
        CZLockGuard() noexcept : didLock(CZLockGuard::Lock()) {}

        /** @brief Unlocks the mutex if this guard locked it. */
        ~CZLockGuard() noexcept
        {
            if (didLock)
                CZLockGuard::Unlock();
        }

        /** @brief Locks the mutex. Returns `true` if locked, `false` if the calling thread already held it. */
        static bool Lock() noexcept;

        /** @brief Unlocks the mutex. Returns `true` if unlocked, `false` if the calling thread didn't hold it. */
        static bool Unlock() noexcept;
    private:
        bool didLock;
    };
}

#endif // CZ_CZLOCKGUARD_H
