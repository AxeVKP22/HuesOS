#include "con.h"

void con() {
    int counter = 0;
    char buffer[64] = {0};

    printChar('>');

    while (1) {
        char key,scancode;
        readKey(&key, &scancode);
        if (scancode == 0x1C) {
            counter = 0;
            break;
        }
        else if (scancode == 0x0E) {
            counter--;
            buffer[counter] = 0;

            printChar('\b');
            printChar(' ');
            printChar('\b');

            continue;
        }

        if (counter < 63) {
            buffer[counter] = key;
            counter++;
            printChar(key);
        }
    }

    newLine();
    
    if (cmpstr("help", buffer) == 0) {
        printString("Available commands:\n\rclear - clears the screen\n\rreboot - reboots the system\n\rhelp - shows this message\n\r");
    }
    else if (cmpstr("reboot", buffer) == 0) {
        reboot();
    }
    else if (cmpstr("clear", buffer) == 0) {
        clearScreen();
    }
    else {
        printString(buffer);
        printString(" : Command not found\n\r");
    }
    return;
}
