#pragma once

#include "sys/disk.h"
#include "sys/mem.h"
#include "type.h"
#include "sys/vga.h"

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

extern char drive;

extern void initFS();

extern void loadFS();
extern void makeFS();
