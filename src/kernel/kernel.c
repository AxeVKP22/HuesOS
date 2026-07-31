#include "../include/sys/keyboard.h"
#include "../include/sys/vga.h"
#include "../include/sys/disk.h"
#include "../include/sys/mem.h"
#include "../include/fs.h"
#include "../include/type.h"
#include "../include/string.h"

#include "../bin/con.h"

const char* kernelMsg = "\n\rHuesOS kernel v0.01, most unstable os ever\n\rAxeVKP22 2026\n\rtype help for command list\n\r";

void kmain(void) {
    clearScreen();
    getDrive(&drive);

    initFS();

    printString(kernelMsg);
    newLine();

    while (1) {
        con();
    }
}


/* 
qemu-system-i386 -drive format=raw,file=build/os.img

ndisasm -b 16 build/boot.bin 
ndisasm -b 16 -o 0x7C00 build/os.img

dd if=/home/axe/gitProjects/wowos/build/os.img of=/dev/sda bs=4M status=progress conv=fsync
stat -c %s build/kernel.bin
*/

