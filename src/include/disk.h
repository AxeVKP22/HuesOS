#pragma once
#include "types.h"


typedef struct {
    uint8_t size;
    uint8_t reserved; // shoud be 0x00
    uint16_t sectors; // sectorsToRead/sectorsToWrite

    uint16_t offset;
    uint16_t segment;

    uint64_t lba;    //startingSector
} DAP;

extern void getDrive(char* drive);
extern int readSectors(uint8_t drive, DAP* dap, uint8_t* err); // return value is 0 if err, 1 if all good  in err stores hex err value
extern int writeSectors(uint8_t drive, DAP* dap, uint8_t* err);// return value is 0 if err, 1 if all good in err stores hex err value