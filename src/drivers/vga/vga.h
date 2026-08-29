#pragma once

#include "include/type.h"
#include "include/sys/io/io.h"
#include "include/lib/memcpy.h"
#include "drivers/ps2/ps2.h"
 
#define FRAMEBUFFER 0xb8000

#define COLOR_BLACK             0x0f
#define COLOR_BLUE              0x1f
#define COLOR_GREEN             0x2f
#define COLOR_CYAN              0x3f
#define COLOR_RED               0x4f
#define COLOR_MAGNETA           0x5f
#define COLOR_BROWN             0x6f
#define COLOR_LIGHT_GREY        0x7f
#define COLOR_DARK_GRAY         0x8f
#define COLOR_LIGHT_BLUE        0x9f
#define COLOR_LIGHT_GREEN       0xaf
#define COLOR_LIGHT_CYAN        0xbf
#define COLOR_LIGHT_RED         0xcf
#define COLOR_LIGHT_MAGNETA     0xdf
#define COLOR_YELLOW            0xef
#define COLOR_WHITE             0xff

extern void putChar(uint8_t Pchar, uint8_t color);
