#include "ps2Keyboard.h"

int ps2ReadKey(uint8_t* scanCode) {
    uint8_t status = inb(0x64);

    if (status & 1) {
        *scanCode = inb(0x60);
    }
    return 0;
}