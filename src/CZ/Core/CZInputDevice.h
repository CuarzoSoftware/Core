#ifndef CZINPUTDEVICE_H
#define CZINPUTDEVICE_H

#include <CZ/Core/CZObject.h>
#include <CZ/Core/CZBitset.h>
#include <memory>
#include <string>

struct libinput_device;
struct wl_pointer;
struct wl_keyboard;
struct wl_touch;

/**
 * @brief Input Device
 */
class CZ::CZInputDevice : public CZObject
{
public:

    /**
     * @brief Input device capabilities
     */
    enum Capability : UInt8
    {
        /// Pointer
        Pointer     = static_cast<UInt8>(1) << 0,

        /// Keyboard
        Keyboard    = static_cast<UInt8>(1) << 1,

        /// Touch
        Touch       = static_cast<UInt8>(1) << 2,

        /// Tablet Tool
        TabletTool  = static_cast<UInt8>(1) << 3,

        /// Tablet Pad
        TabletPad   = static_cast<UInt8>(1) << 4,

        /// Gestures
        Gestures    = static_cast<UInt8>(1) << 5,

        /// Switch
        Switch      = static_cast<UInt8>(1) << 6,
    };

    /** @brief Indicates which member of NativeHandle is valid. */
    enum class NativeHandleType
    {
        None,       ///< No native handle.
        Libinput,   ///< NativeHandle::libinput
        WlPointer,  ///< NativeHandle::wlPointer
        WlKeyboard, ///< NativeHandle::wlKeyboard
        WlTouch,    ///< NativeHandle::wlTouch
        Other       ///< NativeHandle::other
    };

    /** @brief Backend-specific handle of the device, see NativeHandleType. */
    union NativeHandle
    {
        void *other;                ///< Handle of another type.
        libinput_device *libinput;  ///< libinput device.
        wl_pointer *wlPointer;      ///< Wayland pointer.
        wl_keyboard *wlKeyboard;    ///< Wayland keyboard.
        wl_touch *wlTouch;          ///< Wayland touch.
    };

    /** @brief Creates an input device with the given properties. */
    static std::shared_ptr<CZInputDevice> Make(CZBitset<Capability> caps = 0, const std::string &name = "Unknown",
        UInt32 vendorId = 0, UInt32 productId = 0,  NativeHandleType nativeHandleType = NativeHandleType::None, NativeHandle nativeHandle = {}) noexcept
    {
        return std::shared_ptr<CZInputDevice>(new CZInputDevice(caps, name, vendorId, productId, nativeHandleType, nativeHandle));
    }

    /**
     * @brief Checks if the device has the given capability
     */
    CZBitset<Capability> caps;

    /**
     * @brief Name of the input device
     */
    std::string name;

    /**
     * @brief Product ID of the input device
     */
    UInt32 productId;

    /**
     * @brief Vendor ID of the input device
     */
    UInt32 vendorId;

    /**
     * @brief Native data structure used by the current input backend
     *
     * `nullptr` if not available
     */
    NativeHandle nativeHandle;

    /** @brief Which member of @ref nativeHandle is valid. */
    NativeHandleType nativeHandleType;

private:
    CZInputDevice(CZBitset<Capability> caps, const std::string &name, UInt32 vendorId, UInt32 productId, NativeHandleType nativeHandleType, NativeHandle nativeHandle) noexcept :
        caps(caps), name(name), productId(productId), vendorId(vendorId), nativeHandle(nativeHandle), nativeHandleType(nativeHandleType) {}
};


#endif // CZINPUTDEVICE_H
