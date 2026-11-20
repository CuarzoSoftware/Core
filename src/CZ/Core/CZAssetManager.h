#ifndef CZ_CZASSETMANAGER_H
#define CZ_CZASSETMANAGER_H

#include <memory>
#include <unordered_map>
#include <tuple>
#include <functional>

namespace CZ
{

// Base case for recursion
/** @brief Recursion base case of HashCombine(); leaves @p seed unchanged. */
inline void HashCombine(std::size_t &/*seed*/) {}

// Taken from Boost’s hash_combine, which is known for producing good hash distributions
/** @brief Mixes the `std::hash` of each value into @p seed (same formula as Boost's `hash_combine`). */
template<typename T, typename... Rest>
inline void HashCombine(std::size_t &seed, const T &v, const Rest &... rest)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    HashCombine(seed, rest...);
}

// Generic hash for tuple
/** @brief Hash functor for `std::tuple`, combining the hashes of its elements with HashCombine(). */
template<typename... Args>
struct CZTupleHash
{
    /** @brief Returns the combined hash of all elements of @p t. */
    std::size_t operator()(const std::tuple<Args...>& t) const
    {
        std::size_t seed = 0;
        std::apply([&](const auto&... args) { HashCombine(seed, args...); }, t);
        return seed;
    }
};

/**
 * @brief Generic cacheable asset factory
 *
 * Creates assets of type @p T through a factory function and caches them by their creation
 * arguments (@p Args). The cache only holds weak references: an asset is shared while someone
 * keeps a `std::shared_ptr` to it, and recreated by the factory once all references are gone.
 * Expired cache entries are purged periodically on get().
 */
template<typename T, typename... Args>
class CZAssetManager
{
public:
    /** @brief Cache key: the decayed factory arguments. */
    using Key = std::tuple<std::decay_t<Args>...>;

    /** @brief Function that creates a new asset from the arguments, or returns `nullptr` on failure. */
    using FactoryFunc = std::function<std::shared_ptr<T>(Args...)>;

    /** @brief Creates the manager with an optional factory (must be set before calling get()). */
    CZAssetManager(FactoryFunc factory = nullptr) : m_factory(std::move(factory)) {}

    /** @brief Replaces the factory function. */
    void setFactory(FactoryFunc factory) noexcept { m_factory = factory; }

    /**
     * @brief Returns the cached asset for @p args if it is still alive, otherwise creates it with the factory.
     *
     * Failed creations (`nullptr`) are not cached. A factory must be set.
     */
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
