#ifndef CZ_CZEVENT_H
#define CZ_CZEVENT_H

#include <CZ/Core/Cuarzo.h>
#include <CZ/Core/CZTime.h>
#include <type_traits>

#define CZ_GET_CLASS_TYPE(x) std::remove_cv_t<std::remove_reference_t<decltype(*(x))>>
// Used to override copy() in a subclass
#define CZ_EVENT_DECLARE_COPY CZEvent *copy() const noexcept override { return new CZ_GET_CLASS_TYPE(this)(*this); }

/**
 * @brief Base class for events.
 */
class CZ::CZEvent
{
public:

    enum class Type
    {
        /* Cuarzo */

        Destroy,
        Close,
        Presentation,

        Input_First,
        Pointer_First = Input_First,
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
        Input_Last = Touch_Last,

        InputDevicePlugged,
        InputDeviceUnplugged,

        WindowState,

        Render,
        Bake,

        AKSceneChanged,
        Layout,

        Vibrancy,

        /* Louvre */

        LSurfaceCommit,
        LSurfaceUnlock,

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

    bool isInputEvent() const noexcept { return type() >= Type::Input_First && type() <= Type::Input_Last; }
    bool isPointerEvent() const noexcept { return type() >= Type::Pointer_First && type() <= Type::Pointer_Last; };
    bool isKeyboardEvent() const noexcept { return type() >= Type::Keyboard_First && type() <= Type::Keyboard_Last; };
    bool isTouchEvent() const noexcept { return type() >= Type::Touch_First && type() <= Type::Touch_Last; };

    /**
     * @brief Creates a deep copy of the event.
     *
     * @return A pointer to the copied event.
     *
     * @note The returned event must be deleted when no longer used.
     */
    virtual CZEvent *copy() const noexcept = 0;

    bool isAccepted() const noexcept { return m_isAccepted; }
    void accept() const noexcept { m_isAccepted = true; }
    void ignore() const noexcept { m_isAccepted = false; }

    UInt32 serial { CZTime::NextSerial() };
    void *userData {};
protected:
    CZEvent(Type type) noexcept : m_type(type){}
    Type m_type;
    mutable bool m_isAccepted { true };
};

#endif // CZ_CZEVENT_H
