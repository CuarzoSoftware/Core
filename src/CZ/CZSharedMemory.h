#ifndef CZSHAREDMEMORY_H
#define CZSHAREDMEMORY_H

#include <CZObject.h>
#include <memory>
#include <fcntl.h>
#include <sys/mman.h>

class CZ::CZSharedMemory : public CZObject
{
public:
    std::shared_ptr<CZSharedMemory> Make(size_t size, int oflag = O_RDWR | O_CREAT | O_EXCL, mode_t mode = 0666) noexcept;
    int fd() const noexcept { return m_fd; }
    const std::string &name() const noexcept { return m_name; }
    void *map() const noexcept { return m_map; }
    size_t size() const noexcept { return m_size; }
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
