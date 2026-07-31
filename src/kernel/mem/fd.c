#include "../../include/sys/fd.h"

struct descriptor fdTable[32] = {0};
int fdCount = 0;

int newFd(descriptor fd) {
    int curFd = fdCount;
    fdTable[curFd] = fd;
    fdCount++;
    return curFd;
    
}
void closeFd(int fd) {
    descriptor d;
    fdTable[fd] = d;
    fdCount--;
}