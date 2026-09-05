#include "drivers/vga/vga.h"

uint8_t cursorX = 0;
uint8_t cursorY = 0;

void putChar(uint8_t Pchar, uint8_t color) {
    uint16_t pos = cursorY * 80 + cursorX;

    uint8_t data[] = {Pchar, color};
    uint32_t address = FRAMEBUFFER + pos * 2;

    memcpy(data, (void*)address, 2);
    cursorX++;
    if (cursorX >= 80) {
        cursorX = 0;
        cursorY++;
    }
}


void putString(const char* str, uint8_t color) {
    while (*str) {
        putChar(*str++, color);
    }
}

void setCursorPos(uint8_t x, uint8_t y) {
    cursorX = x;
    cursorY = y;

    uint16_t pos = cursorY * 80 + cursorX;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

extern void newLine() {
    cursorX = 0;
    cursorY++;
    if (cursorY >= 25) {
        cursorY = 0;
    }
    uint16_t pos = cursorY * 80 + cursorX;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

