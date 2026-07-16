bits 16

global getDrive
;get drive device from dl register
getDrive:
    push ebp
    mov ebp, esp
    push bx
    
    mov bx, [ebp+8]
    mov [bx], dl

    pop bx
    pop ebp
    ret

global readSectors
readSectors:
    push ebp
    mov ebp, esp
    push si

    mov ah, 42h
    mov dl, [ebp+8]
    mov si, [ebp+12]
    int 13h
    jc .err

    mov eax, 1
    jmp .done

.err:
    mov bx, [ebp+16]
    mov [bx], ah
    xor eax,eax

.done:
    pop si
    pop ebp
    ret

global writeSectors
writeSectors:
    push ebp
    mov ebp, esp
    push bx
    push si

    mov ah, 43h
    mov al, 0x01
    mov dl, [ebp+8]
    mov si, [ebp+12]

    int 13h
    jc .err

    mov eax, 1
    jmp .done

.err:
    mov bx, [ebp+16]
    mov [bx], ah
    xor eax,eax

.done:
    pop si
    pop bx
    pop ebp
    ret


