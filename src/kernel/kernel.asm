bits 16 
[org 0x7e00] 

    call clearScreen

    mov si, kernelMsg
    call printString
    call newLine

    jmp kmain

kmain:
    call con
    jmp kmain

%include "src/bin/con.asm"

kernelMsg db "HuesOS kernel v0.01, most unstable os ever", 0x0d , 0x0a, \
             "AxeVKP22 2026", 0x0d , 0x0a, \
             "type help for command list", 0x0d , 0x0a, 0