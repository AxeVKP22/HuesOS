;a litle console
con:
    ;print ">"
    mov si, conMsg
    call printString

    ;take input until enter is pressed
    call .input

    ;print the input
    mov si, buffer
    call printString

    call newLine

    ;execute the command
    call .exCmd
    
    ;clear the buffer
    mov cx, 64
    mov di, buffer
    xor al, al

    rep stosb
    ret

.input:
    ;take  input and check if enter is pressed
    call readKey
    cmp ah, 0x1C
    ;if yes return to con
    je .enter
    cmp ah, 0x0E
    ;and check if backspace is pressed
    je .backspace

    ;if not write the key to the buffer and increment the counter
    xor bx, bx
    mov bl, [key_counter]
    mov [buffer + bx], al
    inc byte [key_counter]

    ;loop
    jmp .input

.enter:
    ;reset the counter
    mov byte [key_counter], 0
    ret

.backspace:
    ;check if the counter is greater than 0
    cmp byte [key_counter], 0
    jz .input

    ;decrement the counter and clear the last character in the buffer
    dec byte [key_counter]
    xor bx, bx
    mov bl, [key_counter]
    mov byte [buffer + bx], 0
    jmp .input

.exCmd:
    ;check if the command is clear
    mov si, buffer
    mov di, clearCmd
    mov cx, 5
    call cmpStrings
    cmp al ,1
    jz .clearScreen
    
    ;check if the command is reboot
    mov si, buffer
    mov di, rebootCmd
    mov cx, 6
    call cmpStrings
    cmp al ,1
    jz .reboot

    ;check if the command is help
    mov si, buffer
    mov di, helpCmd
    mov cx, 4
    call cmpStrings
    cmp al ,1
    jz .help

    mov si, buffer
    call printString
    mov si, notFoundMsg
    call printString

    ret

.clearScreen:
    call clearScreen
    ret

.reboot:
    call reboot
    ret

.help:
    mov si, helpMsg
    call printString
    ret

%include "src/kernel/vga.asm"
%include "src/kernel/keyboard.asm"
%include "src/kernel/power.asm"

poweroffCmd db "poweroff"
clearCmd db "clear"
rebootCmd db "reboot"
helpCmd db "help"
notFoundMsg db ": Command not found", 0x0d , 0x0a, 0
helpMsg db "Available commands:", 0x0d , 0x0a, \
             "clear - clears the screen", 0x0d , 0x0a, \
             "reboot - reboots the system", 0x0d , 0x0a, \
             "help - shows this message", 0x0d , 0x0a, \
             0

key_counter db 0
buffer times 64 db 0
conMsg db ">", 0