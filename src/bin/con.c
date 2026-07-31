#include "con.h"

void con() {
    int counter = 0;
    char buffer[64] = {0};

    printChar('$');

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
    
    execCommmand(buffer);
    return;
}

int execCommmand(const char* command) {
    int count = 0;
    char buffer[32];
    char args[32];
    while (command[count] != ' ' && command[count] != '\0')
    {
        buffer[count] = command[count];
        count++;
    }
    buffer[count] = '\0';
    if (command[count] == ' ') {
       count++; 
    }
    int i = 0;
    while (command[count] != '\0' && i < sizeof(args) - 1) {
        args[i++] = command[count++];
    }
    args[i] = '\0';
    
    if (cmpstr("help", buffer) == 0) {
        printString("Available commands:\n\rddump [n] - prints data stored in sector n\n\rclear - clears the screen\n\rreboot - reboots the system\n\rhelp - shows this message\n\r");
    }
    else if (cmpstr("reboot", buffer) == 0) {
        reboot();
    }
    else if (cmpstr("clear", buffer) == 0) {
        clearScreen();
    }
    else if (cmpstr("ddump", buffer) == 0) {
        int a = 0;
        int j = 0;

        while (args[j] >= '0' && args[j] <= '9') {
            a = a * 10 + (args[j] - '0');
            j++;
        }
        ddump(a);
    }
    else {
        printString(buffer);
        printString(" : Command not found\n\r");
    }
    return 0;
}