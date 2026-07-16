#include "disk.h"
#include "mem.h"
#include "types.h"
#include "vga.h"

#define MAXFILES 32
#define RESERVED 11

typedef struct FSEntry
{
    char entryName[8];
    char entryExtension[4];
    uint16_t entryLocation;  //n of sector where is entry is stored
    uint16_t entrySize;      //size of the entry, for now max size would be a 512 bytes
} FSEntry;

extern struct FSEntry FSDirTable[MAXFILES];
extern uint8_t sectorMap[63]; //cuz readSectors/writeSectors func can access only first 63 sectors of a disk

extern void initFS(char drive);

extern void loadFS(char drive);
extern void makeFS(char drive);
