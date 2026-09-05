#include "include/lib/memcpy.h"
#include "include/sys/io/io.h"

#include "include/type.h"

#include "drivers/usb/usb.h"
#include "drivers/ps2/ps2.h"
#include "drivers/vga/vga.h"


void kmain(void) {
    uint8_t usbControllerType = initUSB();

    putChar('H', COLOR_MAGNETA);
    putChar('e', COLOR_RED);
    putChar('l', COLOR_GREEN);
    putChar('l', COLOR_CYAN);
    putChar('o', COLOR_LIGHT_BLUE);
    putChar('!', COLOR_BLUE);

    for (;;) {}
}
/*
qemu-system-i386 -device nec-usb-xhci,id=xhci -drive format=raw,file=build/os.img -monitor stdio

ndisasm -b 16 build/boot.bin 
ndisasm -b 16 -o 0x7C00 build/os.img

stat -c %s build/kernel.bin
*/