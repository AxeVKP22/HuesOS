#pragma once

#include "../../type.h"

extern void outb(uint16_t port, uint8_t value);
extern uint8_t inb(uint16_t port);

extern void outl(uint16_t port, uint32_t value);
extern uint32_t inl(uint16_t port);