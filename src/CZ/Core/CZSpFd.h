#ifndef CZ_CZSPFD_H
#define CZ_CZSPFD_H

#include <CZ/Core/CZLog.h>
#include <unistd.h>
#include <fcntl.h>

namespace CZ
{
    /**
     * @brief RAII wrapper for a POSIX file descriptor.
     *
     * Ensures the file descriptor is automatically closed when the object is destroyed.
     * This class is move-only: copy operations are disabled to prevent accidental sharing
     * of the same file descriptor ownership.
     */
    class CZSpFd
    {
    public:
        /**
         * @brief Construct a CZSpFd.
         * @param fd The file descriptor to wrap, or -1 if none.
         */
        explicit CZSpFd(int fd = -1) noexcept : m_fd(fd) {}

        /**
         * @brief Destroy the CZSpFd and close the file descriptor if valid.
         */
        ~CZSpFd() noexcept { close(); }

        CZSpFd(const CZSpFd&) = delete;
        CZSpFd& operator=(const CZSpFd&) = delete;

        /**
         * @brief Move constructor.
         * Transfers ownership from another CZSpFd and invalidates the source.
         * @param other The object to move from.
         */
        CZSpFd(CZSpFd&& other) noexcept : m_fd(other.m_fd)
        {
            other.m_fd = -1;
        }

        /**
         * @brief Move assignment.
         * Closes the current descriptor and transfers ownership from another CZSpFd.
         * @param other The object to move from.
         * @return Reference to this object.
         */
        CZSpFd& operator=(CZSpFd&& other) noexcept
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
        void reset(int fd) noexcept
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
         * @brief Dup the file descriptor.
         *
         * @return The new file descriptor, or -1 on failure.
         */
        [[nodiscard]] CZSpFd dup() const noexcept
        {
            if (m_fd < 0)
                return CZSpFd {};

            const int fd { fcntl(m_fd, F_DUPFD_CLOEXEC, 0) };

            if (fd < 0)
            {
                CZLog(CZError, CZLN, "Failed to dup fd");
                return CZSpFd {};
            }

            return CZSpFd {fd};
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

#endif // CZ_CZSPFD_H
