#ifndef CZ_CZEVENT_H
#define CZ_CZEVENT_H

#include <CZ/Core/Cuarzo.h>
#include <CZ/Core/CZTime.h>
#include <memory>
#include <type_traits>

/**
 * @brief The class type of the object pointed to by @p x, without const/volatile or reference qualifiers.
 */
#define CZ_GET_CLASS_TYPE(x) std::remove_cv_t<std::remove_reference_t<decltype(*(x))>>
/**
 * @brief Implements CZEvent::copy() in a CZEvent subclass.
 *
 * Place it in the public section of every concrete event class.
 */
#define CZ_EVENT_DECLARE_COPY std::shared_ptr<CZEvent> copy() const noexcept override { return std::make_shared<CZ_GET_CLASS_TYPE(this)>(*this); }

/**
 * @brief Base class for events.
 */
class CZ::CZEvent
{
public:

    /**
     * @brief Event types.
     *
     * The `*_First` and `*_Last` values delimit the input, pointer, keyboard and touch ranges
     * (see isInputEvent(), isPointerEvent(), etc.). Custom event types should start at @ref Type::User.
     */
    enum class Type
    {
        /* Cuarzo */

        Destroy, ///< CZDestroyEvent
        Close, ///< CZCloseEvent
        Presentation, ///< CZPresentationEvent

        Input_First, ///< First input event type.
        Pointer_First = Input_First, ///< First pointer event type.
        PointerMove = Pointer_First, ///< CZPointerMoveEvent
        PointerScroll, ///< CZPointerScrollEvent
        PointerButton, ///< CZPointerButtonEvent
        PointerEnter, ///< CZPointerEnterEvent
        PointerLeave, ///< CZPointerLeaveEvent
        PointerSwipeBegin, ///< CZPointerSwipeBeginEvent
        PointerSwipeUpdate, ///< CZPointerSwipeUpdateEvent
        PointerSwipeEnd, ///< CZPointerSwipeEndEvent
        PointerPinchBegin, ///< CZPointerPinchBeginEvent
        PointerPinchUpdate, ///< CZPointerPinchUpdateEvent
        PointerPinchEnd, ///< CZPointerPinchEndEvent
        PointerHoldBegin, ///< CZPointerHoldBeginEvent
        PointerHoldEnd, ///< CZPointerHoldEndEvent
        Pointer_Last = PointerHoldEnd, ///< Last pointer event type.

        Keyboard_First, ///< First keyboard event type.
        KeyboardKey = Keyboard_First, ///< CZKeyboardKeyEvent
        KeyboardModifiers, ///< CZKeyboardModifiersEvent
        KeyboardEnter, ///< CZKeyboardEnterEvent
        KeyboardLeave, ///< CZKeyboardLeaveEvent
        Keyboard_Last = KeyboardLeave, ///< Last keyboard event type.

        Touch_First, ///< First touch event type.
        TouchMove =  Touch_First, ///< CZTouchMoveEvent
        TouchFrame, ///< CZTouchFrameEvent
        TouchDown, ///< CZTouchDownEvent
        TouchUp, ///< CZTouchUpEvent
        TouchCancel, ///< CZTouchCancelEvent
        Touch_Last = TouchCancel, ///< Last touch event type.
        Input_Last = Touch_Last, ///< Last input event type.

        WindowState, ///< CZWindowStateEvent

        Render, ///< Render event (Kay).
        Bake, ///< Bake event (Kay).

        AKSceneChanged, ///< Scene changed event (Kay).
        Layout, ///< CZLayoutEvent

        ColorScheme, ///< CZColorSchemeEvent
        Vibrancy, ///< Vibrancy event (Kay).

        /* Louvre */

        LSurfaceCommit, ///< Surface commit event (Louvre).
        LSurfaceUnlock, ///< Surface unlock event (Louvre).

        User = 1000 ///< First value available for custom event types.
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

    /** @brief Whether the event is an input event (pointer, keyboard or touch, see CZInputEvent). */
    bool isInputEvent() const noexcept { return type() >= Type::Input_First && type() <= Type::Input_Last; }
    /** @brief Whether the event is a pointer event. */
    bool isPointerEvent() const noexcept { return type() >= Type::Pointer_First && type() <= Type::Pointer_Last; };
    /** @brief Whether the event is a keyboard event. */
    bool isKeyboardEvent() const noexcept { return type() >= Type::Keyboard_First && type() <= Type::Keyboard_Last; };
    /** @brief Whether the event is a touch event. */
    bool isTouchEvent() const noexcept { return type() >= Type::Touch_First && type() <= Type::Touch_Last; };

    /**
     * @brief Creates a deep copy of the event.
     *
     * @return A pointer to the copied event.
     *
     * @note The returned event must be deleted when no longer used.
     */
    virtual std::shared_ptr<CZEvent> copy() const noexcept = 0;

    /**
     * @brief Whether the event was accepted.
     *
     * CZCore::sendEvent() marks events as accepted before dispatching them. Receivers can call
     * ignore() to reject an event (e.g. to refuse a CZCloseEvent), which the sender can check here.
     */
    bool isAccepted() const noexcept { return m_isAccepted; }
    /** @brief Marks the event as accepted (see isAccepted()). */
    void accept() const noexcept { m_isAccepted = true; }
    /** @brief Marks the event as not accepted (see isAccepted()). */
    void ignore() const noexcept { m_isAccepted = false; }

    /** @brief Serial number, assigned from CZTime::NextSerial() when the event is created. */
    UInt32 serial { CZTime::NextSerial() };
    /** @brief Custom data attached to the event, not used by Cuarzo itself. */
    void *userData {};
protected:
    /** @brief Constructs an event of the given type. */
    CZEvent(Type type) noexcept : m_type(type){}
    /** @brief The event type (see type()). */
    Type m_type;
    /** @brief Accepted state (see isAccepted()). */
    mutable bool m_isAccepted { true };
};

#endif // CZ_CZEVENT_H
