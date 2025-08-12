#ifndef CZ_CZSCOPEDFD_H
#define CZ_CZSCOPEDFD_H

#include <CZ/CZ.h>
#include <unistd.h>

namespace CZ
{
    /**
     * @brief RAII wrapper for a POSIX file descriptor.
     *
     * Ensures the file descriptor is automatically closed when the object is destroyed.
     * This class is move-only: copy operations are disabled to prevent accidental sharing
     * of the same file descriptor ownership.
     */
    class CZScopedFd
    {
    public:
        /**
         * @brief Construct a CZScopedFd.
         * @param fd The file descriptor to wrap, or -1 if none.
         */
        explicit CZScopedFd(int fd = -1) noexcept : m_fd(fd) {}

        /**
         * @brief Destroy the CZScopedFd and close the file descriptor if valid.
         */
        ~CZScopedFd() noexcept { close(); }

        CZScopedFd(const CZScopedFd&) = delete;
        CZScopedFd& operator=(const CZScopedFd&) = delete;

        /**
         * @brief Move constructor.
         * Transfers ownership from another CZScopedFd and invalidates the source.
         * @param other The object to move from.
         */
        CZScopedFd(CZScopedFd&& other) noexcept : m_fd(other.m_fd)
        {
            other.m_fd = -1;
        }

        /**
         * @brief Move assignment.
         * Closes the current descriptor and transfers ownership from another CZScopedFd.
         * @param other The object to move from.
         * @return Reference to this object.
         */
        CZScopedFd& operator=(CZScopedFd&& other) noexcept
        {
            if (m_fd != other.m_fd)
            {
                close();
                m_fd = other.m_fd;
                other.m_fd = -1;
            }
            return *this;
        }

        /**
         * @brief Get the stored file descriptor.
         *
         * @return The file descriptor, or -1 if none.
         */
        int get() const noexcept { return m_fd; }

        /**
         * @brief Replace the stored file descriptor with a new one.
         *
         * Closes the current descriptor if valid.
         *
         * @param fd The new file descriptor.
         */
        void set(int fd) noexcept
        {
            if (fd == m_fd) return;
            close();
            m_fd = fd;
        }

        /**
         * @brief Release ownership of the descriptor without closing it.
         *
         * The internal value is set to -1.
         *
         * @return The released file descriptor, or -1 if none.
         */
        [[nodiscard]] int release() noexcept
        {
            const int fd { m_fd };
            m_fd = -1;
            return fd;
        }

        /**
         * @brief Close the stored file descriptor if valid.
         *
         * @return true if a descriptor was closed, false if it was already invalid.
         */
        bool close() noexcept
        {
            if (m_fd < 0) return false;
            ::close(m_fd);
            m_fd = -1;
            return true;
        }

    private:
        int m_fd;
    };
}


#endif // CZ_CZSCOPEDFD_H
