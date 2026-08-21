#include "../include/lib/memcpy.h"

void kmain(void) {
    uint8_t data[] = {0x48, 0x4F};
    memcpy(data, (void*)0xB8000, 2);

    for (;;) {}
}

/* 

ndisasm -b 16 build/boot.bin 
ndisasm -b 16 -o 0x7C00 build/os.img

stat -c %s build/kernel.bin
*/