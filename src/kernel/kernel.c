#include "include/lib/memcpy.h"
#include "include/sys/io/io.h"

#include "include/type.h"

#include "drivers/usb/usb.h"


void kmain(void) {
    uint8_t y[] = {0x59, 0x4F};
    uint8_t n[] = {0x4E, 0x4F};
    uint8_t f[] = {0x46, 0x4F};

    uint8_t usbControllerType = initUSB();

    if (usbControllerType == USB_CONTROLLER_XHCI) {
        memcpy(y, (void*)0xB8000, 2);
    } else if (usbControllerType == USB_CONTROLLER_EHCI) {
        memcpy(n, (void*)0xB8000, 2);
    } else if (usbControllerType == USB_CONTROLLER_NOT_FOUND) {
        memcpy(f, (void*)0xB8000, 2);
    }

    for (;;) {}
}
/*
qemu-system-i386 -device nec-usb-xhci,id=xhci -drive format=raw,file=build/os.img -monitor stdio

ndisasm -b 16 build/boot.bin 
ndisasm -b 16 -o 0x7C00 build/os.img

stat -c %s build/kernel.bin
*/