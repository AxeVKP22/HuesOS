#include "../type.h"
#include "disk.h"

#define MAXFD 32

#define SEGMENT 0x0000
#define STARTINGADDR 0x0500
#define SECTOR 0x200

typedef struct fileDescriptor
{
    uint16_t segment;
    uint16_t offset;            //addr where file is lod
} fileDescriptor;

extern int newFd(const struct fileDescriptor* fd);
extern void closeFd(int fd);

extern uint16_t calcFdOffset();