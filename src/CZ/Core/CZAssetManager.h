#ifndef CZ_CZASSETMANAGER_H
#define CZ_CZASSETMANAGER_H

#include <memory>
#include <unordered_map>
#include <tuple>
#include <functional>

namespace CZ
{

// Base case for recursion
inline void HashCombine(std::size_t &/*seed*/) {}

// Taken from Boost’s hash_combine, which is known for producing good hash distributions
template<typename T, typename... Rest>
inline void HashCombine(std::size_t &seed, const T &v, const Rest &... rest)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    HashCombine(seed, rest...);
}

// Generic hash for tuple
template<typename... Args>
struct CZTupleHash
{
    std::size_t operator()(const std::tuple<Args...>& t) const
    {
        std::size_t seed = 0;
        std::apply([&](const auto&... args) { HashCombine(seed, args...); }, t);
        return seed;
    }
};

/**
 * @brief Generic cacheable asset factory
 */
template<typename T, typename... Args>
class CZAssetManager
{
public:
    using Key = std::tuple<std::decay_t<Args>...>;
    using FactoryFunc = std::function<std::shared_ptr<T>(Args...)>;

    CZAssetManager(FactoryFunc factory = nullptr) : m_factory(std::move(factory)) {}
    void setFactory(FactoryFunc factory) noexcept { m_factory = factory; }

    std::shared_ptr<T> get(Args... args)
    {
        assert(m_factory && "Missing factory function");

        ++m_accessCount;

        // Occasionally clean expired
        if (m_accessCount % CleanupInterval == 0)
            cleanupExpired();

        Key key(args...);

        auto it = m_cache.find(key);

        if (it != m_cache.end())
        {
            if (auto existing = it->second.lock())
                return existing;
            else
                m_cache.erase(it);
        }

        // Create new asset
        auto asset = m_factory(args...);

        if (!asset) return {};

        m_cache[key] = asset;
        return asset;
    }

private:

    void cleanupExpired()
    {
        for (auto it = m_cache.begin(); it != m_cache.end();)
        {
            if (it->second.expired())
                it = m_cache.erase(it);
            else
                ++it;
        }
    }
    std::unordered_map<Key, std::weak_ptr<T>, CZTupleHash<Args...>> m_cache;
    FactoryFunc m_factory;
    size_t m_accessCount {};
    static constexpr size_t CleanupInterval = 100;
};
}

#endif // CZ_CZASSETMANAGER_H
