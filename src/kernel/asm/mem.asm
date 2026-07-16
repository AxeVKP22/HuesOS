bits 16

global memcpyToRam
memcpyToRam:
    push ebp
    mov ebp, esp

    push si
    push di
    push ds
    push es

    mov si, [ebp+8]
    mov ax, [ebp+12]
    mov es, ax
    
    mov di, [ebp+16]
    mov cx, [ebp+20]

    cld
    rep movsb

    pop es
    pop ds
    pop di
    pop si

    pop ebp
    ret


global memcpyToBuff
memcpyToBuff:
    push ebp
    mov ebp, esp

    push si
    push di
    push ds
    push es

    mov ax, [ebp+12]
    mov ds, ax
    mov si, [ebp+16]

    push cs
    pop es
    mov di, [ebp+8] 

    mov cx, [ebp+20]

    cld
    rep movsb

    pop es
    pop ds
    pop di
    pop si

    pop ebp
    ret

