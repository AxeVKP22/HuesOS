#include "usbCore.h"

static struct USBcontroller findController() {

    struct USBcontroller usbcontroller;

    for (uint16_t bus = 0;bus < 256;bus++) {
        for (uint8_t device = 0;device < 32;device++) {
            for (uint8_t function = 0;function < 8;function++) {

                outl(0xCF8, (1 << 31) | (bus << 16) | (device << 11) | (function << 8));
                uint32_t data = inl(0xCFC);

                if (data == 0xFFFFFFFF) {
                    continue;
                }

                outl(0xCF8, (1 << 31) | (bus << 16) | (device << 11) | (function << 8) | 0x08);
                data = inl(0xCFC);

                uint8_t classCode = (data >> 24) & 0xFF;
                uint8_t subclassCode = (data >> 16) & 0xFF;
                uint8_t progIF = (data >> 8) & 0xFF;

                if (classCode == 0x0C && subclassCode == 0x03) {
                    if (progIF == 0x30) {
                        usbcontroller.bus = bus;
                        usbcontroller.device = device;
                        usbcontroller.function = function;
                        usbcontroller.classCode = classCode;
                        usbcontroller.subclassCode = subclassCode;
                        usbcontroller.progIF = progIF;

                        return usbcontroller;
                    } else if (progIF == 0x20) {
                        usbcontroller.bus = bus;
                        usbcontroller.device = device;
                        usbcontroller.function = function;
                        usbcontroller.classCode = classCode;
                        usbcontroller.subclassCode = subclassCode;
                        usbcontroller.progIF = progIF;

                        return usbcontroller;
                    }
                }
            }
        }
    }
    usbcontroller.progIF = USB_CONTROLLER_NOT_FOUND;;

    return usbcontroller;
}

struct USBcontroller initUSB() {
    struct USBcontroller usbcontroller = findController();

    if (usbcontroller.progIF == USB_CONTROLLER_XHCI) {
        initXHCI(&usbcontroller);
        return usbcontroller;
    } else if (usbcontroller.progIF == USB_CONTROLLER_EHCI) {
        return usbcontroller;
    } else if (usbcontroller.progIF == USB_CONTROLLER_NOT_FOUND) {
        return usbcontroller;
    }
}