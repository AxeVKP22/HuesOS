#include "kernel/ints/pic/pic.h"

void remapPic() {
    outb(MASTER_CMD, 0x11);
    outb(SLAVE_CMD, 0x11);

    outb(MASTER_DATA, 0x20);
    outb(SLAVE_DATA, 0x28);

    outb(MASTER_DATA, 0x04);
    outb(SLAVE_DATA, 0x02);

    outb(MASTER_DATA, 0x01);
    outb(SLAVE_DATA, 0x01);

    outb(MASTER_DATA, 0xFD);
    outb(SLAVE_DATA, 0xFF);
}