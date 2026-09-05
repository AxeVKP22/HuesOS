#pragma once 

#include "include/type.h"
#include "include/sys/io/io.h"
#include "include/lib/memcpy.h"

#include "drivers/vga/vga.h"

#define BUFFER_SIZE 32

typedef struct {
    uint8_t scancode;
    const uint8_t *name;
    uint8_t ascii;
} Key;

static const Key keyboard[128] = {
    [0x00] = {0x00, "NONE",       0},

    [0x01] = {0x01, "ESC",        0},

    [0x02] = {0x02, "1",          '1'},
    [0x03] = {0x03, "2",          '2'},
    [0x04] = {0x04, "3",          '3'},
    [0x05] = {0x05, "4",          '4'},
    [0x06] = {0x06, "5",          '5'},
    [0x07] = {0x07, "6",          '6'},
    [0x08] = {0x08, "7",          '7'},
    [0x09] = {0x09, "8",          '8'},
    [0x0A] = {0x0A, "9",          '9'},
    [0x0B] = {0x0B, "0",          '0'},
    [0x0C] = {0x0C, "MINUS",      '-'},
    [0x0D] = {0x0D, "EQUAL",      '='},
    [0x0E] = {0x0E, "BACKSPACE",  0},
    [0x0F] = {0x0F, "TAB",        0},

    [0x10] = {0x10, "Q",          'q'},
    [0x11] = {0x11, "W",          'w'},
    [0x12] = {0x12, "E",          'e'},
    [0x13] = {0x13, "R",          'r'},
    [0x14] = {0x14, "T",          't'},
    [0x15] = {0x15, "Y",          'y'},
    [0x16] = {0x16, "U",          'u'},
    [0x17] = {0x17, "I",          'i'},
    [0x18] = {0x18, "O",          'o'},
    [0x19] = {0x19, "P",          'p'},
    [0x1A] = {0x1A, "LBRACKET",   '['},
    [0x1B] = {0x1B, "RBRACKET",   ']'},
    [0x1C] = {0x1C, "ENTER",      0},
    [0x1D] = {0x1D, "LCTRL",      0},

    [0x1E] = {0x1E, "A",          'a'},
    [0x1F] = {0x1F, "S",          's'},
    [0x20] = {0x20, "D",          'd'},
    [0x21] = {0x21, "F",          'f'},
    [0x22] = {0x22, "G",          'g'},
    [0x23] = {0x23, "H",          'h'},
    [0x24] = {0x24, "J",          'j'},
    [0x25] = {0x25, "K",          'k'},
    [0x26] = {0x26, "L",          'l'},
    [0x27] = {0x27, "SEMICOLON",  ';'},
    [0x28] = {0x28, "APOSTROPHE", '\''},
    [0x29] = {0x29, "GRAVE",      '`'},

    [0x2A] = {0x2A, "LSHIFT",     0},
    [0x2B] = {0x2B, "BACKSLASH",  '\\'},

    [0x2C] = {0x2C, "Z",          'z'},
    [0x2D] = {0x2D, "X",          'x'},
    [0x2E] = {0x2E, "C",          'c'},
    [0x2F] = {0x2F, "V",          'v'},
    [0x30] = {0x30, "B",          'b'},
    [0x31] = {0x31, "N",          'n'},
    [0x32] = {0x32, "M",          'm'},
    [0x33] = {0x33, "COMMA",      ','},
    [0x34] = {0x34, "DOT",        '.'},
    [0x35] = {0x35, "SLASH",      '/'},

    [0x36] = {0x36, "RSHIFT",     0},
    [0x37] = {0x37, "KPSTAR",     '*'},
    [0x38] = {0x38, "LALT",       0},
    [0x39] = {0x39, "SPACE",      ' '},
    [0x3A] = {0x3A, "CAPSLOCK",   0},

    [0x3B] = {0x3B, "F1",         0},
    [0x3C] = {0x3C, "F2",         0},
    [0x3D] = {0x3D, "F3",         0},
    [0x3E] = {0x3E, "F4",         0},
    [0x3F] = {0x3F, "F5",         0},
    [0x40] = {0x40, "F6",         0},
    [0x41] = {0x41, "F7",         0},
    [0x42] = {0x42, "F8",         0},
    [0x43] = {0x43, "F9",         0},
    [0x44] = {0x44, "F10",        0},

    [0x45] = {0x45, "NUMLOCK",    0},
    [0x46] = {0x46, "SCROLLLOCK", 0},

    [0x47] = {0x47, "KP7_HOME",   '7'},
    [0x48] = {0x48, "KP8_UP",     '8'},
    [0x49] = {0x49, "KP9_PGUP",   '9'},
    [0x4A] = {0x4A, "KPMINUS",    '-'},
    [0x4B] = {0x4B, "KP4_LEFT",   '4'},
    [0x4C] = {0x4C, "KP5",        '5'},
    [0x4D] = {0x4D, "KP6_RIGHT",  '6'},
    [0x4E] = {0x4E, "KPPLUS",     '+'},
    [0x4F] = {0x4F, "KP1_END",    '1'},
    [0x50] = {0x50, "KP2_DOWN",   '2'},
    [0x51] = {0x51, "KP3_PGDN",   '3'},
    [0x52] = {0x52, "KP0_INSERT", '0'},
    [0x53] = {0x53, "KPDOT_DEL",  '.'},

    [0x54] = {0x54, "SYSRQ",      0},
    [0x55] = {0x55, "NONE",       0},
    [0x56] = {0x56, "OEM102",     0},

    [0x57] = {0x57, "F11",        0},
    [0x58] = {0x58, "F12",        0},

    [0x59] = {0x59, "NONE",       0},
    [0x5A] = {0x5A, "NONE",       0},
    [0x5B] = {0x5B, "LGUI",       0},
    [0x5C] = {0x5C, "RGUI",       0},
    [0x5D] = {0x5D, "MENU",       0},

    [0x5E] = {0x5E, "NONE",       0},
    [0x5F] = {0x5F, "NONE",       0},
    [0x60] = {0x60, "NONE",       0},
    [0x61] = {0x61, "NONE",       0},
    [0x62] = {0x62, "NONE",       0},
    [0x63] = {0x63, "NONE",       0},
    [0x64] = {0x64, "NONE",       0},
    [0x65] = {0x65, "NONE",       0},
    [0x66] = {0x66, "NONE",       0},
    [0x67] = {0x67, "NONE",       0},
    [0x68] = {0x68, "NONE",       0},
    [0x69] = {0x69, "NONE",       0},
    [0x6A] = {0x6A, "NONE",       0},
    [0x6B] = {0x6B, "NONE",       0},
    [0x6C] = {0x6C, "NONE",       0},
    [0x6D] = {0x6D, "NONE",       0},
    [0x6E] = {0x6E, "NONE",       0},
    [0x6F] = {0x6F, "NONE",       0},
    [0x70] = {0x70, "NONE",       0},
    [0x71] = {0x71, "NONE",       0},
    [0x72] = {0x72, "NONE",       0},
    [0x73] = {0x73, "NONE",       0},
    [0x74] = {0x74, "NONE",       0},
    [0x75] = {0x75, "NONE",       0},
    [0x76] = {0x76, "NONE",       0},
    [0x77] = {0x77, "NONE",       0},
    [0x78] = {0x78, "NONE",       0},
    [0x79] = {0x79, "NONE",       0},
    [0x7A] = {0x7A, "NONE",       0},
    [0x7B] = {0x7B, "NONE",       0},
    [0x7C] = {0x7C, "NONE",       0},
    [0x7D] = {0x7D, "NONE",       0},
    [0x7E] = {0x7E, "NONE",       0},
    [0x7F] = {0x7F, "NONE",       0}
};

extern const Key* ps2ReadKey(void);
extern void ps2Init(void);