#include "../include/fs.h"

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

static void writeFSDirTable() {
    char err;
    memcpyToRam(&FSDirTable, 0x0000, 0x9000, sizeof(FSDirTable));
    dap.lba = 20;
    if (writeSectors(drive, &dap, &err) == 0) {
        printString("err in loading FS dirTable on disk\n\r");
        printHex(err);
        newLine();
    }
}

static void writeSectorMap() {
    char err;
    memcpyToRam(&sectorMap, 0x0000, 0x9000, sizeof(sectorMap));
    dap.lba = 21;
    if (writeSectors(drive, &dap, &err) == 0) {
        printString("err in loading FS sectorMap on disk\n\r");
        printHex(err);
        newLine();
    }
}

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
    writeFSDirTable();

    for (int i = 0;i<RESERVED;i++) {
        sectorMap[i] = 1; //reserve sectors 0-11
        //1-bootloader
        //2-7 kernel 
        //8-19 reserved for kernel
        //20 dirTable
        //21 sectorMap
    }

    writeSectorMap();
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


int sysOpen(const char* filename, uint8_t flags) {
    for (int i = 0;i<MAXFILES;i++) {
        if (cmpstr(FSDirTable[i].entryName, filename) == 0) {

            uint16_t offset = calcOffset();

            if (offset == 0xFFFF) {
                return -1;
            }

            DAP dap = {
                .size = 16,
                .reserved = 0,
                .sectors = 1,
                .segment = 0x0000,
                .offset = offset,
                .lba = FSDirTable[i].entryLocation
            };

            if (readSectors(drive, &dap, 0) == 0) {
                return -1;
            }

            fileDescriptor fd = {
                .offset = offset,
                .segment = 0x0000,
                .flags = flags,
                .used = 0x01,
                .fileLocation = i
            };
            
            return allocateFd(&fd);
        }
    }
    return -1;
}

int sysNew(const char* filename) {
    int dirTableIndex = -1;

    for (int i = 0;i<MAXFILES;i++) {
        if (FSDirTable[i].entryUsed == 0x00) {
            dirTableIndex = i;
            break;
        }
    }

    if (dirTableIndex == -1) {
        newLine();
        printString("Too many files");
        return -1;
    }

    int sector = -1;

    for (int i = 0;i<sizeof(sectorMap);i++) {
        if (sectorMap[i] == 0x00) {
            sector = i;
            sectorMap[i] = 0x01;
            break;
        }
    }
    if (sector == -1) {
        newLine();
        printString("Disk full");
        return -1;
    }

    writeSectorMap();

    FSEntry entry = {
        .entryUsed = 0x01,
        .entryLocation = sector,
        .entrySize = 0
    };

    for (int i = 0; i < 11; i++) {
        entry.entryName[i] = filename[i];
        if (filename[i] == '\0') {
            break;
        }
    }

    FSDirTable[dirTableIndex] = entry;

    writeFSDirTable();

    return dirTableIndex;
}


int sysClose(int fd) {
    fileDescriptor cFd = getFd(fd);
    if (cFd.flags & O_SAVE) {
        DAP dap = {
            .size = 16,
            .reserved = 0,
            .sectors = 1,
            .segment = 0x0000,
            .offset = cFd.offset,
            .lba = FSDirTable[cFd.fileLocation].entryLocation
        };

        char err;
        if (writeSectors(drive, &dap, &err) == 0) {
            printString("Error writing file to disk\n\r");
            printHex(err);
            newLine();
            return -1;
        }
    }

    freeFd(fd);
    return 0;
}


int sysWrite(int fd, void* buffer, uint16_t size) {
    fileDescriptor wFd = getFd(fd);

    if (!(wFd.flags & O_WRITE)) {
        return -1;
    }
    memcpyToRam(buffer, wFd.segment, wFd.offset, size);
    return size;
}

int sysRead(int fd, void* buffer, uint16_t size) {
    fileDescriptor rFd = getFd(fd);

    if (!(rFd.flags & O_READ)) {
        return -1;
    }
    memcpyToBuff(buffer, rFd.segment, rFd.offset, size);
    return size;
}