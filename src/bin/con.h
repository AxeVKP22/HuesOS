#pragma once

#include "../include/sys/keyboard.h"
#include "../include/sys/vga.h"
#include "../include/sys/power.h"
#include "../include/string.h"

#include "ddump.h"

extern void con();
extern int execCommmand(const char* command);