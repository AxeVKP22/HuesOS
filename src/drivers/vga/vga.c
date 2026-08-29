#include "drivers/vga/vga.h"

uint32_t offset = 0x0000;

void putChar(uint8_t Pchar, uint8_t color) {
    uint8_t data[] = {Pchar, color};
    uint32_t address = FRAMEBUFFER + offset;
    memcpy(data, (void*)address, 2);
    offset += 2;
}