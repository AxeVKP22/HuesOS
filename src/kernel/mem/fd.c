#include "../../include/sys/fd.h"

struct fileDescriptor fdTable[MAXFD] = {0};
uint8_t fdMap[MAXFD] = {0};

fileDescriptor getFd(int fd) {
    return fdTable[fd];
}

int newFd(const struct fileDescriptor* fd) {

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

void closeFd(int fd) {
    
}

uint16_t calcFdOffset() {
    for (int i = 0;i<MAXFD;i++) {
        if (fdMap[i] == 0x00) {
            fdMap[i] = 0x01;
            return STARTINGADDR + SECTOR * i;
        }
    }
   return 0xFFFF; //err
}