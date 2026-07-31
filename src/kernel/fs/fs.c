#include "../../include/fs.h"

struct FSEntry FSDirTable[MAXFILES] = {0};
uint8_t sectorMap[63] = {0};

char drive;

DAP dap = {
        .size = 16,
        .reserved = 0,
        .sectors = 0,
        .segment = 0x0000,
        .offset = 0x9000,
        .lba = 0
    };


static void loadFS() {
    dap.lba = 20;
    readSectors(drive, &dap, 0);
    memcpyToBuff(FSDirTable, 0x0000, 0x9000, sizeof(FSDirTable));   //read sector 20 to 0000:9000 and cpy it to loadedDirTable;
                                                                    //sector 10 must contain dirTable
    dap.lba = 21;
                                                                    //maybe later i add err log but now i dont have issues with this
    readSectors(drive, &dap, 0);
    memcpyToBuff(sectorMap, 0x0000, 0x9000, sizeof(sectorMap));     //read sector 21 to 0000:9000 and cpy it to loadedSectorMap;
}
                                                                    //and sector 11 must contain sectorMap
static void makeFS() {
    uint8_t header[2] = {0xAF,0x22};
    char err;

    //load FS header to sector 2
    memcpyToRam(&header, 0x0000, 0x9000, sizeof(header));
    dap.lba = 1;
    if (writeSectors(drive, &dap, &err) == 0) {
        printString("err in loading FS header on disk\n\r");
        printHex(err);
        newLine();
    }

    // make FS structures on disk
    memcpyToRam(&FSDirTable, 0x0000, 0x9000, sizeof(FSDirTable));
    dap.lba = 20;
    if (writeSectors(drive, &dap, &err) == 0) {
        printString("err in loading FS dirTable on disk\n\r");
        printHex(err);
        newLine();
    }

    for (int i = 0;i<RESERVED;i++) {
        sectorMap[i] = 1; //reserve sectors 0-11
        //1-bootloader
        //2-7 kernel 
        //8-19 reserved for kernel
        //20 dirTable
        //21 sectorMap
    }

    memcpyToRam(&sectorMap, 0x0000, 0x9000, sizeof(sectorMap));
    dap.lba = 21;
    if (writeSectors(drive, &dap, &err) == 0) {
        printString("err in loading FS sectorMap on disk\n\r");
        printHex(err);
        newLine();
    }
}

void initFS() {
    //check for existing FS header
    char FSHeader[2];
    dap.sectors = 1;
    dap.lba = 1;

    if (readSectors(drive, &dap, 0) == 0) {                 //read sector 2 and load to 0000:9000 this sector must contain FS header;
        printString("Err in loading 2nd sector\n\r");
    } 
    else {
        printString("2nd sector is loaded\n\r");
    }
    memcpyToBuff(&FSHeader, 0x0000, 0x9000, 2);             // and copy it from 0000:9000 to FSHeader arr

    //check for FS signature
    if (FSHeader[0] == -81 && FSHeader[1] == 34) {
        printString("FS header found\n\r");
        printString("Loading FS\n\r");
        loadFS(drive);
    }
    else {
        printString("FS header not found\n\r");
        printString("Creating FS\n\r");
        makeFS(drive);
    }
}

int open(const char* filename) {
    int curFile;

    for (int i = 0;i<MAXFILES;i++) {
        if (FSDirTable[i].entryName == filename) {
            uint16_t segment = 0x0000; //hardcoded for now;
            uint16_t offset = 0x0500;

            DAP dap = {
                .size = 16,
                .reserved = 0,
                .sectors = 1,
                .segment = segment,
                .offset = offset,
                .lba = FSDirTable[i].entryLocation
            };

            readSectors(drive, &dap, 0);

            descriptor fd = {
                .entrySize = FSDirTable[i].entrySize,
                .segment = segment,
                .offset = offset
                
            };
            return newFd(fd);
        }
        else {

        }
    }
}