#include "ps2Keyboard.h"

const Key* ps2ReadKey(void)
{
    if (!(inb(0x64) & 1))
        return 0;

    uint8_t scanCode = inb(0x60);

    if (scanCode & 0x80)
        return 0;

    return &keyboard[scanCode];
}