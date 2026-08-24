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
    int argc = 0;

    char buffer[32];
    char args[3][32];

    while (command[count] != ' ' && command[count] != '\0') {
        buffer[count] = command[count];
        count++;
    }

    buffer[count] = '\0';

    while (command[count] == ' ') {
        count++;
    }

    while (command[count] != '\0' && argc < 3) {
        int i = 0;

        while (command[count] != ' ' &&
               command[count] != '\0' &&
               i < 31) {
            args[argc][i++] = command[count++];
        }

        args[argc][i] = '\0';
        argc++;

        while (command[count] == ' ') {
            count++;
        }
    }
    
    if (cmpstr("help", buffer) == 0) {
        printString("Available commands:\n\r"
            "\n\r[DEBUG]\n\r"
            "ddump [n] - prints data stored in sector [n]\n\r"
            "\n\r[FS]\n\r"
            "touch [filename] - creates a new file with name [filename]\n\r"
            "cat [filename] - prints content of the [filename]\n\r"
            "write [filename] [data] - write a [data] to [filename]\n\r"
            "\n\r[CONSOLE]\n\r"
            "clear - clears the screen\n\r"
            "\n\r[POWER]\n\r"
            "reboot - reboots the system\n\r"
            "\n\r[OTHER]\n\r"
            "help - shows this message\n\r");
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

        while (args[0][j] >= '0' && args[0][j] <= '9') {
            a = a * 10 + (args[0][j] - '0');
            j++;
        }

        ddump(a);
    }

    else if (cmpstr("touch", buffer) == 0) {
        int fd = sysNew(args[0]);
        if (fd != -1) {
            printString("File created successfully\n\r");
        }
    }

    else if (cmpstr("cat", buffer) == 0) {
        uint8_t fd = sysOpen(args[0], O_RDWR | O_SAVE);
        char readBuffer[MAXSIZE];

        if (fd != -1) {
            sysRead(fd, readBuffer, MAXSIZE);
            sysClose(fd);
        }

        for (int i = 0;i<MAXSIZE;i++) {
            printChar(readBuffer[i]);
        }

        newLine();
    }

    else if (cmpstr("write", buffer) == 0) {
        int fd = sysOpen(args[0], O_RDWR | O_SAVE);

        if (fd != -1) {
            int len = length(args[1]) / 2;
            uint8_t data[len];

            for (int i = 0; i < len; i++) {
                char h = args[1][i * 2];
                char l = args[1][i * 2 + 1];

                uint8_t high;
                uint8_t low;

                if (h >= '0' && h <= '9')
                    high = h - '0';
                else if (h >= 'A' && h <= 'F')
                    high = h - 'A' + 10;
                else
                    high = h - 'a' + 10;

                if (l >= '0' && l <= '9')
                    low = l - '0';
                else if (l >= 'A' && l <= 'F')
                    low = l - 'A' + 10;
                else
                    low = l - 'a' + 10;

                data[i] = (high << 4) | low;
            }

            if (sysWrite(fd, data, len) == len)
                printString("success");

            sysClose(fd);
        }

        newLine();
    }

    else {
        printString(buffer);
        printString(" : Command not found\n\r");
    }
    return 0;
}