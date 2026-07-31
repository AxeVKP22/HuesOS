#pragma once

#include "sys/disk.h"
#include "sys/mem.h"
#include "type.h"
#include "sys/vga.h"
#include "sys/fd.h"
#include "string.h"

#define MAXFILES 32
#define RESERVED 22

typedef struct FSEntry
{
    uint8_t entryUsed;  //0x00 no, 0x01 yes;
    char entryName[11];
    uint16_t entryLocation;  //n of sector where is entry is stored
    uint16_t entrySize;      //size of the entry, for now max size would be a 512 bytes
} FSEntry;

extern struct FSEntry FSDirTable[MAXFILES];
extern uint8_t sectorMap[63];

extern char drive;

extern void initFS();

extern int open(const char* filename);
extern int new(const char* filename);