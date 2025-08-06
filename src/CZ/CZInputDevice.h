#ifndef CZINPUTDEVICE_H
#define CZINPUTDEVICE_H

#include <CZ/CZObject.h>
#include <CZ/CZBitset.h>
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

    enum class NativeHandleType
    {
        None,
        Libinput,
        WlPointer,
        WlKeyboard,
        WlTouch,
        Other
    };

    union NativeHandle
    {
        void *other;
        libinput_device *libinput;
        wl_pointer *wlPointer;
        wl_keyboard *wlKeyboard;
        wl_touch *wlTouch;
    };

    static std::shared_ptr<CZInputDevice> Make(CZBitset<Capability> caps = 0, const std::string &name = "Unknown",
        UInt32 vendorId = 0, UInt32 productId = 0,  NativeHandleType nativeHandleType = NativeHandleType::None, void *nativeHandle = nullptr) noexcept
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

    NativeHandleType nativeHandleType;

private:
    CZInputDevice(CZBitset<Capability> caps, const std::string &name, UInt32 vendorId, UInt32 productId, NativeHandleType nativeHandleType, void *nativeHandle) noexcept :
        caps(caps), name(name), productId(productId), vendorId(vendorId), nativeHandle(nativeHandle), nativeHandleType(nativeHandleType) {}
};


#endif // CZINPUTDEVICE_H
