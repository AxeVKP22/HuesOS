#include "../../include/sys/fd.h"

struct fileDescriptor fdTable[MAXFD] = {0};
uint8_t fdMap[MAXFD] = {0};

fileDescriptor getFd(int fd) {
    return fdTable[fd];
}

int allocateFd(const struct fileDescriptor* fd) {

    int retFd = -1;

    for (int i = 0;i<MAXFD;i++) {
        if (fdTable[i].used == 0x00) {
            fdTable[i] = *fd;
            fdTable[i].used = 0x01;
            return i;
        }
    }
    return -1;
}

void freeFd(int fd) {
    fileDescriptor emptyFd = {
        .used = 0x00,
        .segment = 0x0000,
        .offset = 0x0000
    };

    int fdMapIndex = (fdTable[fd].offset - STARTINGADDR) / SECTOR;
    fdMap[fdMapIndex] = 0x00;
    fdTable[fd] = emptyFd;
}

uint16_t calcOffset() {
    for (int i = 0;i<MAXFD;i++) {
        if (fdMap[i] == 0x00) {
            fdMap[i] = 0x01;
            return STARTINGADDR + SECTOR * i;
        }
    }
   return 0xFFFF; //err
}