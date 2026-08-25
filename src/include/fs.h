#pragma once

#include "sys/disk.h"
#include "sys/mem.h"
#include "type.h"
#include "sys/vga.h"
#include "sys/fd.h"
#include "string.h"

#define MAXFILES 32
#define RESERVED 27
#define MAXSIZE 512

#define ROOT_TABLE 25
#define SECTOR_MAP 26

#define O_READ 0x01
#define O_WRITE 0x02
#define O_RDWR (O_READ | O_WRITE)
#define O_SAVE 0x04

typedef struct FSEntry
{
    uint8_t entryUsed;       //0x00 no, 0x01 yes;
    char entryName[11];
    uint16_t entryLocation;  //n of sector where is entry is stored
    uint16_t entrySize;      //size of the entry, for now max size would be a 512 bytes
} FSEntry;

extern struct FSEntry rootTable[MAXFILES];
extern struct FSEntry* currTableP;
extern uint8_t sectorMap[512];

extern char workingDir[32];

extern char drive;

extern void initFS();

extern int sysOpen(const char* filename, uint8_t flags);
extern int sysNew(const char* filename);

extern int sysClose(int fd);

extern int sysWrite(int fd, void* buffer, uint16_t size);
extern int sysRead(int fd, void* buffer, uint16_t size);

extern int sysDelete(const char* filename);

extern int sysGetSize(int fd);