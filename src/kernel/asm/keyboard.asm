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

;si - 1st string
;di - 2nd string
;cx - length of the strings
global cmpStrings
cmpStrings:
    push ebp
    mov ebp, esp

    push si
    push di

    mov si, [ebp+8]
    mov di, [ebp+12]
    mov cx, [ebp+16]

    cld
    repe cmpsb
    jne .not_equal

    mov eax,1
    jmp .done

.not_equal:
    xor eax,eax

.done:
    pop di
    pop si
    pop ebp
    ret