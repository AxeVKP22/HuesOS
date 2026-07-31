#include "../../include/sys/fd.h"

static struct fileDescriptor fdTable[MAXFD] = {0};
static uint8_t fdMap[MAXFD] = {0};

static uint8_t fdCount = 0;

int newFd(const struct fileDescriptor* fd) {
    if (fdCount >= MAXFD)
        return -1;

    int currFd = fdCount;
    fdTable[fdCount] = *fd;
    fdCount++;
    return currFd;

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