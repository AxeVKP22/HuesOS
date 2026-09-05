#pragma once

#include "include/type.h"
#include "include/sys/io/io.h"
#include "include/lib/memcpy.h"

#include "drivers/ps2/ps2.h"

#define IDT_SIZE 256

struct IDTPointer {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

struct IDTEntry {
    uint16_t offsetLow;
    uint16_t selector;
    uint8_t  zero;
    uint8_t  typeAttr;
    uint16_t offsetHigh;
} __attribute__((packed));

extern struct IDTEntry idt[IDT_SIZE];

extern void initIDT();

extern void setGate(uint8_t index, uint32_t handler);
extern void lidt(struct IDTPointer* idtr);