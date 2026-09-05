#pragma once 

#include "drivers/ps2/ps2.h"

//my fukin motherboard doesnt support legacy usb, and ive just wasted time by writing this driver

extern void ps2KeyboardHandler(void);
extern void ps2Handler(void);