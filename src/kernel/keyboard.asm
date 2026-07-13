;al - char
;ah - scancode
readKey:
    mov ah, 0
    int 16h
    ret

;si - 1st string
;di - 2nd string
;cx - length of the strings
cmpStrings:
    cld
    repe cmpsb
    je .equal
    xor al, al
    ret

.equal:
    mov al, 1
    ret
    