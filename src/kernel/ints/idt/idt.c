#include "kernel/ints/idt/idt.h"


struct IDTEntry idt[IDT_SIZE] = {0};

void setGate(uint8_t index, uint32_t handler) {
    idt[index].offsetLow = (handler & 0xFFFF);
    idt[index].offsetHigh = ((handler >> 16) & 0xFFFF);
    idt[index].selector = 0x08;
    idt[index].zero = 0;
    idt[index].typeAttr = 0x8E;
}

void initIDT() {
    struct IDTPointer idtP;

    setGate(0x21, (uint32_t)ps2Handler);

    idtP.limit = sizeof(idt) - 1;
    idtP.base = (uint32_t)&idt;

    lidt(&idtP);
}