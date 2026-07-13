;all of ts just a interrupt calls

;bh - page n
;dh - row
;dl - collumn
setCursorPos:
    mov ah, 02h
    int 0x10

;dh - row
;dl - collumn
getCursorPos:
    mov ah, 03h
    mov bh, 0
    int 0x10

;al - char
printChar:
    mov ah, 0x0e
    int 0x10
    ret

;si - string
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