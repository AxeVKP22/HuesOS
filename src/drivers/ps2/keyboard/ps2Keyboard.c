#include "ps2Keyboard.h"

volatile uint8_t ps2KeyBuffer = 0x00;

const Key* ps2ReadKey(void) {
    while (ps2KeyBuffer == 0x00) {}

    uint8_t scanCode = ps2KeyBuffer;
    ps2KeyBuffer = 0x00;

    return &keyboard[scanCode];
}

void ps2KeyboardHandler(void) {
    uint8_t scanCode = inb(0x60);

    if (!(scanCode & 0x80)) {
        ps2KeyBuffer = scanCode;
    }

    outb(0x20, 0x20);
}

void ps2Init(void) {
    outb(0x64, 0xAA);

    while (inb(0x64) & 0x00) {
        
    }
    uint8_t status = inb(0x60);
    if (status != 0x55) {
        putString("PS/2 Controller self-test failed!", COLOR_RED);
        newLine();

    } else {
        putString("PS/2 Controller self-test passed!", COLOR_GREEN);
        newLine();
    }
}