#include "ps2Keyboard.h"

uint8_t ps2KeyBuffer[BUFFER_SIZE];

uint8_t bufferRead = 0;
uint8_t bufferWrite = 0;
uint8_t bufferCount = 0;

const Key* ps2ReadKey(void) {
    while (bufferCount == 0) {}

    uint8_t scanCode = ps2KeyBuffer[bufferRead];
    putChar(keyboard[scanCode].ascii, COLOR_CYAN);

    bufferRead++;
    if (bufferRead >= BUFFER_SIZE)
        bufferRead = 0;

    bufferCount--;

    return &keyboard[scanCode];
}

void ps2KeyboardHandler(void) {
    uint8_t scanCode = inb(0x60);

    if (!(scanCode & 0x80)) {

        if (bufferCount < BUFFER_SIZE) {
            ps2KeyBuffer[bufferWrite] = scanCode;

            bufferWrite++;
            if (bufferWrite >= BUFFER_SIZE)
                bufferWrite = 0;

            bufferCount++;
        }
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