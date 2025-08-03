#ifndef CZEVENT_H
#define CZEVENT_H

#include <CZ/Cuarzo.h>

/* Used to override copy() in a subclass */
#define CZ_EVENT_DECLARE_COPY CZEvent *copy() const noexcept override { return new CZ_GET_CLASS_TYPE(this)(*this); }

/**
 * @brief Base class for events.
 */
class CZ::CZEvent
{
public:

    /**
     * @brief Defines the type of event.
     */
    enum Type
    {
        Destroy,

        Pointer_First,
        PointerMove = Pointer_First,
        PointerScroll,
        PointerButton,
        PointerEnter,
        PointerLeave,
        PointerSwipeBegin,
        PointerSwipeUpdate,
        PointerSwipeEnd,
        PointerPinchBegin,
        PointerPinchUpdate,
        PointerPinchEnd,
        PointerHoldBegin,
        PointerHoldEnd,
        Pointer_Last = PointerHoldEnd,

        Keyboard_First,
        KeyboardKey = Keyboard_First,
        KeyboardModifiers,
        KeyboardEnter,
        KeyboardLeave,
        Keyboard_Last = KeyboardLeave,

        Touch_First,
        TouchMove =  Touch_First,
        TouchFrame,
        TouchDown,
        TouchUp,
        TouchCancel,
        Touch_Last = TouchCancel,

        WindowState,
        WindowClose,

        Render,
        Bake,

        SceneChanged,
        Layout,

        Vibrancy,

        User = 1000
    };

    /**
     * @brief Destructor.
     */
    virtual ~CZEvent() noexcept = default;

    /**
     * @brief Retrieves the type of the event.
     */
    Type type() const noexcept { return m_type; }

    /**
     * @brief Checks if the event type is any of the given types.
     *
     * @return `true` if any of the types match, `false` otherwise.
     */
    template<typename... Types>
    constexpr bool typeIsAnyOf(Types...types) const noexcept
    {
        for (const auto t : {types...})
            if (t == type())
                return true;
        return false;
    }

    bool isPointer() const noexcept { return type() >= Pointer_First && type() <= Pointer_Last; };
    bool isKeyboard() const noexcept { return type() >= Keyboard_First && type() <= Keyboard_Last; };
    bool isTouch() const noexcept { return type() >= Touch_First && type() <= Touch_Last; };

    UInt32 serial {};
    void *userData {};

    /**
     * @brief Creates a deep copy of the event.
     *
     * @return A pointer to the copied event.
     *
     * @note The returned event must be deleted when no longer used.
     */
    virtual CZEvent *copy() const noexcept { return new CZEvent(*this); }

    bool isAccepted() const noexcept { return m_isAccepted; }
    void accept() const noexcept { m_isAccepted = true; }
    void ignore() const noexcept { m_isAccepted = false; }

protected:
    CZEvent(Type type, UInt32 serial) noexcept :
        m_type(type),
        m_serial(serial)
    {}
    Type m_type;
    UInt32 m_serial;
    mutable bool m_isAccepted { true };
};

/**
 * @}
 */

#endif // CZ_CORE_EVENT_H
