global getDrive
;get drive device from dl register
getDrive:
    push ebp
    mov ebp, esp

    mov [ebp+8], dl

    pop ebp
    ret

global readSectors
readSectors:
    push ebp
    mov ebp, esp

    mov ah, 02h
    mov al, [ebp+8]
    mov ch, 0         ;only if reading sectors 1-63
    mov cl, [ebp+12]
    mov dh, 0         ;only if reading sectors 1-63
    mov dl, [ebp+16]

    mov ax, [ebp+20]
    mov es, ax
    mov bx, [ebp+24]

    int 13h
    jc .err

    mov eax, 1
    jmp .done

.err:
    xor eax,eax

.done:
    pop ebp
    ret

global writeSectors
writeSectors:
    push ebp
    mov ebp, esp

    mov ah, 03h
    mov al, [ebp+8]
    mov ch, 0             ;only if writing sectors 1-63
    mov cl, [ebp+12]
    mov dh, 0             ;only if writing sectors 1-63
    mov dl, [ebp+16]

    mov ax, [ebp+20]
    mov es, ax
    mov bx, [ebp+24]

    int 13h
    jc .err

    mov eax, 1
    jmp .done

.err:
    xor eax,eax

.done:
    pop ebp
    ret


