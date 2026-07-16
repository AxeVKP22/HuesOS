bits 16
;al - char
;ah - scancode
global readKey
readKey:
    push ebp
    mov ebp, esp

    push bx

    mov ah, 0
    int 16h

    mov bx, [ebp+8]
    mov [bx], al

    mov bx, [ebp+12]
    mov [bx], ah

    pop bx
    pop ebp
    ret