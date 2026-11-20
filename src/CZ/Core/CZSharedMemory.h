#ifndef CZSHAREDMEMORY_H
#define CZSHAREDMEMORY_H

#include <CZ/Core/CZObject.h>
#include <memory>
#include <fcntl.h>
#include <sys/mman.h>

/**
 * @brief POSIX shared memory object mapped into the process.
 *
 * Created with a random unique name (`/cz_shm_<n>`) via `shm_open()`, resized and mapped read/write.
 * On destruction the memory is unmapped, the fd is closed and the name is unlinked.
 */
class CZ::CZSharedMemory : public CZObject
{
public:
    /**
     * @brief Creates and maps a shared memory object of @p size bytes.
     *
     * @param size Size in bytes.
     * @param oflag Flags passed to `shm_open()`.
     * @param mode Permissions passed to `shm_open()`.
     * @return The shared memory, or `nullptr` on failure.
     */
    static std::shared_ptr<CZSharedMemory> Make(size_t size, int oflag = O_RDWR | O_CREAT | O_EXCL | O_CLOEXEC, mode_t mode = 0666) noexcept;

    /** @brief File descriptor of the shared memory object. */
    int fd() const noexcept { return m_fd; }

    /** @brief Name of the shared memory object, as passed to `shm_open()`. */
    const std::string &name() const noexcept { return m_name; }

    /** @brief Address of the read/write mapping. */
    void *map() const noexcept { return m_map; }

    /** @brief Size in bytes. */
    size_t size() const noexcept { return m_size; }

    /**
     * @brief Resizes the object and remaps it. map() may change.
     *
     * @return `true` on success. On failure the previous mapping may already have been released.
     */
    bool resize(size_t size) noexcept;
    ~CZSharedMemory() noexcept;
private:
    CZSharedMemory(size_t size, int fd, void *map, const std::string &name) noexcept :
        m_size(size), m_fd(fd), m_map(map), m_name(name) {}
    size_t m_size;
    int m_fd;
    void *m_map;
    std::string m_name;
};

#endif // CZSHAREDMEMORY_H
