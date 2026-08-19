#include "../type.h"
#include "disk.h"

#define MAXFD 32

#define SEGMENT 0x0000
#define STARTINGADDR 0x0500
#define SECTORSIZE 0x200

typedef struct fileDescriptor
{   
    uint8_t flags;
    uint8_t used;               //0x00 no; 0x01 yes
    uint16_t fileLocation;

    uint16_t segment;
    uint16_t offset;            //addr where file is lod
} fileDescriptor;

extern fileDescriptor getFd(int fd);
extern int allocateFd(const struct fileDescriptor* fd);
extern void freeFd(int fd);

extern uint16_t calcOffset();