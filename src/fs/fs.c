#include "../include/fs.h"

struct FSEntry rootTable[MAXFILES] = {0};
struct FSEntry currTable[MAXFILES] = {0};
int currTableLoc = ROOT_TABLE;

struct FSEntry* currTableP = rootTable;
uint8_t sectorMap[512] = {0};

char workingDir[32] = "root.dir";

char drive;

DAP dap = {
    .size = 16,
    .reserved = 0,
    .sectors = 0,
    .segment = 0x0000,
    .offset = 0x9200,
    .lba = 0
};

static void writeFSDirTable() {
    char err;
    memcpyToRam(currTableP, 0x0000, 0x9200, 512);
    dap.lba = currTableLoc;
    if (writeSectors(drive, &dap, &err) == 0) {
        printString("err in loading FS dirTable on disk\n\r");
        printHex(err);
        newLine();
    }
}

static void writeSectorMap() {
    char err;
    memcpyToRam(&sectorMap, 0x0000, 0x9200, sizeof(sectorMap));
    dap.lba = SECTOR_MAP;
    if (writeSectors(drive, &dap, &err) == 0) {
        printString("err in loading FS sectorMap on disk\n\r");
        printHex(err);
        newLine();
    }
}

static void loadFS() {
    dap.lba = ROOT_TABLE;
    readSectors(drive, &dap, 0);
    memcpyToBuff(currTableP, 0x0000, 0x9200, 512);   //read sector 20 to 0000:9000 and cpy it to loadedDirTable;
                                                                    //sector 10 must contain dirTable
    dap.lba = SECTOR_MAP;
                                                                    //maybe later i add err log but now i dont have issues with this
    readSectors(drive, &dap, 0);
    memcpyToBuff(sectorMap, 0x0000, 0x9200, sizeof(sectorMap));     //read sector 21 to 0000:9000 and cpy it to loadedSectorMap;
}
                                                                    //and sector 11 must contain sectorMap
static void makeFS() {
    uint8_t header[2] = {0xAF, 0x22};
    char err;

    memcpyToRam(header, 0x0000, 0x9200, sizeof(header));
    dap.lba = 1;

    if (writeSectors(drive, &dap, &err) == 0) {
        printString("err in loading FS header on disk\n\r");
        printHex(err);
        newLine();
    }

    rootTable[0].entryUsed = 0x01;
    rootTable[0].entryLocation = ROOT_TABLE;
    rootTable[0].entrySize = 0;
    rootTable[0].entryName[0] = '.';
    rootTable[0].entryName[1] = '\0';

    rootTable[1].entryUsed = 0x01;
    rootTable[1].entryLocation = ROOT_TABLE;
    rootTable[1].entrySize = 0;
    rootTable[1].entryName[0] = '.';
    rootTable[1].entryName[1] = '.';
    rootTable[1].entryName[2] = '\0';

    currTableP = rootTable;
    currTableLoc = ROOT_TABLE;

    writeFSDirTable();

    for (int i = 0; i < RESERVED; i++) {
        sectorMap[i] = 1;
    }

    writeSectorMap();
}

static void addWorkingDir(const char* dirname) {
    int dirLen = length(dirname);
    int currentLen = length(workingDir);

    if (currentLen + dirLen + 1 >= sizeof(workingDir))
        return;

    workingDir[currentLen] = '/';

    for (int i = 0; i < dirLen; i++) {
        workingDir[currentLen + 1 + i] = dirname[i];
    }

    workingDir[currentLen + 1 + dirLen] = '\0';
}

static void removeWorkingDir() {
    int len = length(workingDir);

    if (len <= 8)
        return;

    for (int i = len - 1; i >= 0; i--) {
        if (workingDir[i] == '/') {
            workingDir[i] = '\0';
            return;
        }
    }
}

void initFS() {
    //check for existing FS header
    char FSHeader[2];
    dap.sectors = 1;
    dap.lba = 1;

    workingDir[0] = 'r';
    workingDir[1] = 'o';
    workingDir[2] = 'o';
    workingDir[3] = 't';
    workingDir[4] = '.';
    workingDir[5] = 'd';
    workingDir[6] = 'i';
    workingDir[7] = 'r';
    workingDir[8] = '\0';

    if (readSectors(drive, &dap, 0) == 0) {                 //read sector 2 and load to 0000:9000 this sector must contain FS header;
        printString("Err in loading 2nd sector\n\r");
    } 
    else {
        printString("2nd sector is loaded\n\r");
    }
    memcpyToBuff(&FSHeader, 0x0000, 0x9200, 2);             // and copy it from 0000:9000 to FSHeader arr

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
    currTableP = rootTable;
    
}


int sysOpen(const char* filename, uint8_t flags) {

    if (cmpstr(filename, "..") == 0) {

        if (currTableLoc == ROOT_TABLE)
            return -2;

        uint16_t parent = currTableP[1].entryLocation;

        writeFSDirTable();

        dap.sectors = 1;
        dap.lba = parent;

        if (readSectors(drive, &dap, 0) == 0)
            return -1;

        if (parent == ROOT_TABLE) {
            memcpyToBuff(rootTable, 0x0000, 0x9200, 512);
            currTableP = rootTable;
        } else {
            memcpyToBuff(currTable, 0x0000, 0x9200, 512);
            currTableP = currTable;
        }

        currTableLoc = parent;
        removeWorkingDir();

        return -2;
    }

    for (int i = 2; i < MAXFILES; i++) {

        if (!currTableP[i].entryUsed)
            continue;

        if (cmpstr(currTableP[i].entryName, filename) != 0)
            continue;

        int len = length(filename);

        if (len >= 4 && filename[len - 4] == '.' && filename[len - 3] == 'd' && filename[len - 2] == 'i' && filename[len - 1] == 'r') {

            writeFSDirTable();

            dap.sectors = 1;
            dap.lba = currTableP[i].entryLocation;

            if (readSectors(drive, &dap, 0) == 0)
                return -1;

            memcpyToBuff(currTable, 0x0000, 0x9200, 512);

            currTableLoc = currTableP[i].entryLocation;
            currTableP = currTable;

            addWorkingDir(filename);

            return -2;
        }

        uint16_t offset = calcOffset();

        if (offset == 0xFFFF)
            return -1;

        DAP fileDap = {
            .size = 16,
            .reserved = 0,
            .sectors = 1,
            .segment = 0x0000,
            .offset = offset,
            .lba = currTableP[i].entryLocation
        };

        if (readSectors(drive, &fileDap, 0) == 0)
            return -1;

        fileDescriptor fd = {
            .offset = offset,
            .segment = 0x0000,
            .flags = flags,
            .used = 0x01,
            .fileLocation = i,
            .fileSize = currTableP[i].entrySize,
            .fileSector = currTableP[i].entryLocation
        };

        return allocateFd(&fd);
    }

    return -1;
}

int sysNew(const char* filename) {
    int dirTableIndex = -1;

    for (int i = 2; i < MAXFILES; i++) {
        if (currTableP[i].entryUsed == 0x00) {
            dirTableIndex = i;
            break;
        }
    }

    if (dirTableIndex == -1) {
        printString("Too many files");
        return -1;
    }

    for (int i = 2; i < MAXFILES; i++) {
        if (currTableP[i].entryUsed &&
            cmpstr(currTableP[i].entryName, filename) == 0) {
            return -1;
        }
    }

    int sector = -1;

    for (int i = 0; i < sizeof(sectorMap); i++) {
        if (sectorMap[i] == 0x00) {
            sector = i;
            sectorMap[i] = 0x01;
            break;
        }
    }

    if (sector == -1) {
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

        if (filename[i] == '\0')
            break;
    }

    currTableP[dirTableIndex] = entry;

    writeFSDirTable();

    int len = length(filename);

    if (len >= 4 && filename[len - 4] == '.' && filename[len - 3] == 'd' && filename[len - 2] == 'i' && filename[len - 1] == 'r') {

        FSEntry newDir[MAXFILES] = {0};

        newDir[0].entryUsed = 0x01;
        newDir[0].entryLocation = sector;
        newDir[0].entrySize = 0;

        newDir[0].entryName[0] = '.';
        newDir[0].entryName[1] = '\0';

        newDir[1].entryUsed = 0x01;
        newDir[1].entryLocation = currTableLoc;
        newDir[1].entrySize = 0;

        newDir[1].entryName[0] = '.';
        newDir[1].entryName[1] = '.';
        newDir[1].entryName[2] = '\0';

        char err;

        memcpyToRam(newDir, 0x0000, 0x9200, 512);

        dap.sectors = 1;
        dap.lba = sector;

        if (writeSectors(drive, &dap, &err) == 0) {
            printString("Error creating directory\n\r");
            printHex(err);
            newLine();
            return -1;
        }
    }

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
            .lba = cFd.fileSector
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

int sysDelete(const char* filename) {
    for (int i = 2;i<MAXFILES;i++) {
        if (cmpstr(currTable[i].entryName, filename) == 0) {
            FSEntry empty = {
                .entryLocation = 0x00,
                .entryName = 0x00,
                .entryUsed = 0x00,
                .entrySize = 0x00
            };
            sectorMap[currTable[i].entryLocation] = 0x00;
            currTable[i] = empty;
            writeFSDirTable();
            writeSectorMap();
        }
    }
}

int sysGetSize(int fd) {
    return getFd(fd).fileSize;
}