bits 16
[org 0x7e00]

start:

    ;load kernel to 0x8000
    mov ah, 0x02
    mov al, 1         ; sectors to read
    mov ch, 0          ; cylinder
    mov cl, 3          ; sector (starts at 1)
    mov dh, 0          ; head

    xor bx, bx
    mov es, bx
    mov bx, 0x8000     ; load address

    int 0x13
    jc disk_error

    mov ah, 0x0e
    mov al, 's'     ; this code is so unstable that without printring s kernel will not load :/
    int 0x10

    ;enbale protected mdoe
    cli 
    in al, 0x92
    and al, 0xFE
    or al, 2
    out 0x92, al 

    lgdt [gdtDescriptor]

    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp 0x08:protectedMode

disk_error:
    mov [error_code], ah

    mov al, [error_code]
    call print_hex

    cli
    hlt

print_hex:
    pusha

    mov bl, al

    mov al, bl
    shr al, 4
    call print_nibble

    mov al, bl
    and al, 0x0f
    call print_nibble

    popa
    ret

; print low nibble in AL
print_nibble:
    cmp al, 9
    jbe .digit

    add al, 'A' - 10
    jmp .print

.digit:
    add al, '0'

.print:
    mov ah, 0x0e
    int 0x10
    ret

error_code db 0




;jmp to kerne;
bits 32
protectedMode:
    mov ax, 0x10
    mov ds, ax
    mov es, ax  
    mov ss, ax
    mov esp, 0x90000

    mov esi, 0x8000
    mov edi, 0x100000

    mov ecx, 512/4
    rep movsd

    jmp 0x08:0x100000


gdtStart:

gdtNull:
    dq 0x0000000000000000

gdtKernelCode:
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 0x9a
    db 0xcf
    db 0x00

gdtKernelData:
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 0x92
    db 0xcf
    db 0x00

gdtUserCode:
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 0xfa
    db 0xcf
    db 0x00

gdtUserData:
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 0xf2
    db 0xcf
    db 0x00

gdtEnd:

gdtDescriptor:
    dw gdtEnd - gdtStart - 1
    dd gdtStart

times 512-($-$$) db 0