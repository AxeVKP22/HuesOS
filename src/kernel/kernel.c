#include "../include/lib/memcpy.h"
#include "../include/sys/io/io.h"

void kmain(void) {
    uint8_t data[] = {0x48, 0x4F};
    memcpy(data, (void*)0xB8000, 2);

    uint8_t key = inb(0x60);

    memcpy(&key, (void*)0xB8000, 1);

    for (;;) {}
}

/* 

ndisasm -b 16 build/boot.bin 
ndisasm -b 16 -o 0x7C00 build/os.img

stat -c %s build/kernel.bin
*/