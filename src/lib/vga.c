#include "../include/sys/vga.h"

void printHex(char hex) {
    char h[] = "0123456789ABCDEF";

    printChar(h[(hex >> 4) & 0xF]);
    printChar(h[hex & 0xF]);
}