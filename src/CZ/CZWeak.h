#ifndef CZWEAK_H
#define CZWEAK_H

#include <CZ/CZ.h>
#include <functional>

class CZ::CZWeakUtils
{
public:
    static std::vector<void *> &objectRefs(const CZObject *object) noexcept;
    static bool isObjectDestroyed(const CZObject *object) noexcept;
};

/**
 * @brief Weak reference to an CZObject
 *
 * The CZWeak class template provides a mechanism for creating weak pointer references to subclasses of CZObject.\n
 * It is conceptually similar to `std::weak_ptr` but is specifically tailored for CZObject subclasses, avoiding
 * the pointer indirection and associated performance overhead of the `std::weak_ptr` thread-safe mechanisms.
 *
 * When the object being referenced is destroyed, an optional on destroy callback event is emitted, and get() returns `nullptr`.
 */
template <class T>
class CZ::CZWeak
{
public:

    /**
     * Callback function type used to handle the `OnDestroy()` event.
     */
    using OnDestroyCallback = std::function<void(T*)>;

    /**
     * @brief Creates an empty CZWeak
     */
    CZWeak() noexcept = default;

    /**
     * @brief Creates a reference for the given CZObject, or initializes an empty CZWeak if `nullptr` is passed.
     *
     * @param object The CZObject to create a reference for, or `nullptr` to initialize an empty CZWeak.
     */
    CZWeak(T *object) noexcept
    {
        static_assert(std::is_base_of<CZObject, T>::value, "CZWeak template error: T must be a subclass of CZObject.");

        if (object)
            pushBackTo(object);
    }

    /**
     * @brief Destructor, removes the CZWeak from the CZObject references.
     */
    ~CZWeak() noexcept
    {
        clear();

        if (m_onDestroyCallback)
            delete m_onDestroyCallback;
    }

    /**
     * @brief Copy constructor, assigns the CZObject of another CZWeak.
     *
     * @param other The CZWeak object to copy from.
     */
    CZWeak(const CZWeak<T> &other) noexcept
    {
        copy(other);
    }

    /**
     * @brief Assignment operator, assigns the CZObject of another CZWeak.
     *
     * @param other The CZWeak object to assign from.
     * @return Reference to the updated CZWeak object.
     */
    CZWeak<T> &operator=(const CZWeak<T> &other) noexcept
    {
        copy(other);
        return *this;
    }

    /**
     * @brief Gets a pointer to the CZObject or `nullptr` if not set or the object has been destroyed.
     *
     * @return Raw pointer to the referenced CZObject.
     */
    T *get() const noexcept
    {
        return m_object;
    }

    /**
     * @brief Implicit conversion to raw pointer.
     *
     * Provides access to the underlying raw pointer through an implicit conversion.
     *
     * @return A pointer to the underlying object.
     */
    operator T*() const noexcept
    {
        return m_object;
    }

    /**
     * @brief Access underlying object via pointer semantics.
     *
     * Allows accessing members of the underlying object using pointer semantics.
     *
     * @return Pointer to the underlying object.
     */
    T* operator->() const noexcept
    {
        return m_object;
    }

    /**
     * @brief Return the number of existing references to the current CZObject.
     *
     * @return The number of existing references to the current CZObject, if no object is set returns 0.
     */
    UInt64 count() const noexcept
    {
        if (m_object)
        {
            const auto &refs = (std::vector<CZWeak<T>*>&)CZWeakUtils::objectRefs((const CZObject*)m_object);
            return refs.size();
        }

        return 0;
    }

    /**
     * @brief Replace the reference with another object.
     *
     * @param object The CZObject to set as the new reference, or `nullptr` to unset the reference.
     */
    void reset(T *object = nullptr) noexcept
    {
        static_assert(std::is_base_of<CZObject, T>::value, "CZWeak template error: Type must be a subclass of CZObject.");

        clear();

        if (object)
            pushBackTo(object);
    }

    /**
     * @brief Set the onDestroy callback function.
     *
     * @note callback functions are not copied across CZWeak instances.
     *
     * @param callback The callback function to be called when the referenced object is destroyed. Passing `nullptr` disables the callback.
     */
    void setOnDestroyCallback(const OnDestroyCallback &callback) noexcept
    {
        if (m_onDestroyCallback)
        {
            delete m_onDestroyCallback;
            m_onDestroyCallback = nullptr;
        }

        if (callback)
            m_onDestroyCallback = new OnDestroyCallback(callback);
    }

private:
    friend class CZObject;

    void copy(const CZWeak<T> &other) noexcept
    {
        clear();

        if (other.m_object)
            pushBackTo(other.m_object);
    }

    void clear() noexcept
    {
        if (m_object)
        {
            auto &refs = (std::vector<CZWeak<T>*>&)CZWeakUtils::objectRefs((const CZObject*)m_object);
            refs.back()->m_index = m_index;
            refs[m_index] = refs.back();
            refs.pop_back();
            m_object = nullptr;
        }
    }

    void pushBackTo(T *object) noexcept
    {
        if (CZWeakUtils::isObjectDestroyed((const CZObject*)object))
            return;

        m_object = object;
        auto &refs = (std::vector<CZWeak<T>*>&)CZWeakUtils::objectRefs((const CZObject*)m_object);
        refs.push_back(this);
        m_index = refs.size() - 1;
    }

    T *m_object { nullptr };
    UInt64 m_index { 0 };
    OnDestroyCallback *m_onDestroyCallback { nullptr };
};

#endif
