;all of ts just a interrupt calls

printString:
    lodsb
    cmp al, 0
    je .done
    mov ah, 0x0e
    int 0x10
    jmp printString
.done:
    ret

newLine:
    mov al, 0x0d
    mov ah, 0x0e
    int 10h

    mov al, 0x0a
    mov ah, 0x0e
    int 10h
    ret

clearScreen:
    mov ah, 0x00
    mov al, 0x03
    int 0x10
    ret