#include <CZ/Core/CZSharedMemory.h>
#include <random>

using namespace CZ;

static std::string GenerateUniqueName() noexcept
{
    static std::mt19937 rng(std::random_device{}());
    static std::uniform_int_distribution<int> dist(0, 999999);
    return "/cz_shm_" + std::to_string(dist(rng));
}

std::shared_ptr<CZSharedMemory> CZSharedMemory::Make(size_t size, int oflag, mode_t mode) noexcept
{
    for (int attempts = 0; attempts < 999999; attempts++)
    {
        const std::string name { GenerateUniqueName() };

        const int fd { shm_open(name.c_str(), oflag, mode) };

        if (fd < 0)
            continue;

        if (ftruncate(fd, size) < 0)
        {
            ::close(fd);
            shm_unlink(name.c_str());
            continue;
        }

        void *addr { mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0) };

        if (addr == MAP_FAILED)
        {
            ::close(fd);
            shm_unlink(name.c_str());
            continue;
        }

        return std::shared_ptr<CZSharedMemory>(new CZSharedMemory(size, fd, addr, name));
    }

    return {};
}

bool CZSharedMemory::resize(size_t newSize) noexcept
{
    if (newSize == m_size)
        return true;

    if (munmap(m_map, m_size) < 0)
        return false;

    if (ftruncate(m_fd, newSize) < 0)
        return false;

    void *newMap { mmap(nullptr, newSize, PROT_READ | PROT_WRITE, MAP_SHARED, m_fd, 0) };

    if (newMap == MAP_FAILED)
        return false;

    m_map = newMap;
    m_size = newSize;
    return true;
}

CZSharedMemory::~CZSharedMemory() noexcept
{
    if (m_map)
        munmap(m_map, m_size);

    if (m_fd >= 0)
        ::close(m_fd);

    if (!m_name.empty())
        shm_unlink(m_name.c_str());
}
