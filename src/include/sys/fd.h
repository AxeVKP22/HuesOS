#include "../type.h"

typedef struct descriptor
{
    uint16_t segment;
    uint16_t offset;

    uint16_t entrySize;      //size of the entry, for now max size would be a 512 bytes
} descriptor;

extern struct descriptor fdTable[32];
extern int fdCount;

extern int newFd(descriptor fd);
extern void closeFd(int fd);