#include "../include/keyboard.h"
#include "../include/vga.h"
#include "../include/disk.h"
#include "../include/mem.h"
#include "../include/fs.h"
#include "../include/types.h"

#include "../bin/con.h"

const char* kernelMsg = "HuesOS kernel v0.01, most unstable os ever\n\rAxeVKP22 2026\n\rtype help for command list\n\r";
char drive = 0;

void kmain(void) {
    clearScreen();
    getDrive(&drive);

    initFS(drive);
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
*/

