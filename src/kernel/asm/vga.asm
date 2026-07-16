bits 16
;all of ts just a interrupt calls

global setCursorPos
;bh - page n
;dh - row
;dl - collumn
setCursorPos:
    push ebp
    mov ebp, esp

    mov bh, [ebp+8]
    mov dh, [ebp+12]
    mov dl , [ebp+16]

    mov ah, 02h
    int 0x10

    pop ebp
    ret

global getCursorPos
;dh - row
;dl - collumn
getCursorPos:
    push ebp
    mov ebp, esp

    push bx
    
    mov ah, 03h
    mov bh, 0
    int 0x10

    mov bx, [ebp+8]
    mov [bx], dh

    mov bx, [ebp+12]
    mov [bx], dl

    pop bx
    pop ebp
    ret

global printChar
;al - char
printChar:
    push ebp
    mov ebp, esp

    mov al, [ebp+8]

    mov ah, 0x0e
    int 0x10

    pop ebp
    ret

global printString
;si - string
printString:
    push ebp
    mov ebp, esp

    push si
    mov si, [ebp+8]

.loop:
    lodsb
    cmp al, 0
    je .done

    mov ah, 0x0e
    int 0x10

    jmp .loop

.done:
    pop si
    pop ebp
    ret

global newLine
newLine:
    mov al, 0x0d
    mov ah, 0x0e
    int 10h

    mov al, 0x0a
    mov ah, 0x0e
    int 10h
    ret

global clearScreen
clearScreen:
    mov ah, 0x00
    mov al, 0x03
    int 0x10
    ret
