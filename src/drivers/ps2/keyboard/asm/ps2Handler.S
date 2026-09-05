bits 32

#include "drivers/ps2/keyboard/asm/ps2Handler.h"

extern ps2KeyboardHandler
global ps2Handler

ps2Handler:
    pushad
    call ps2KeyboardHandler
    popad
    iret