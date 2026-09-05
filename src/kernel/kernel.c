#include "include/lib/memcpy.h"
#include "include/sys/io/io.h"
#include "include/sys/sti.h"

#include "include/type.h"

#include "drivers/usb/usb.h"
#include "drivers/ps2/ps2.h"
#include "drivers/vga/vga.h"

#include "kernel/ints/pic/pic.h"
#include "kernel/ints/idt/idt.h"

void kmain(void) {
    remapPic();
    initIDT();
    sti();
    struct USBcontroller usbControllerType = initUSB();
    ps2Init();

    putString("HuesOS 32-bit Kernel Initialized!", COLOR_GREEN);
    newLine();

    for (;;) {
        Key* key = ps2ReadKey();
        putChar(key->ascii, COLOR_BLACK);
    }
}
/*
qemu-system-i386 -device nec-usb-xhci,id=xhci -drive format=raw,file=build/os.img -monitor stdio
qemu-system-i386 -drive format=raw,file=build/os.img -monitor stdio

ndisasm -b 16 build/boot.bin 
ndisasm -b 16 -o 0x7C00 build/os.img

stat -c %s build/kernel.bin
*/