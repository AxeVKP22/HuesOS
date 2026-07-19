#include "ddump.h"

void ddump(int sector) {
    DAP dap = {
        .size = 16,
        .reserved = 0,
        .sectors = 1,
        .segment = 0x0000,
        .offset = 0x9000,
        .lba = 0
    };

    uint8_t buffer[512];
    
    dap.lba = sector;
    readSectors(drive, &dap, 0);
    memcpyToBuff(buffer, 0x0000, 0x9000, 512);
    for (int i = 0; i<sizeof(buffer);i++) {
        printHex(buffer[i]);
        //printString("  ");
    }
    newLine();
}