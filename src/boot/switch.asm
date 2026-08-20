bits 16
[org 0x7e00]

start:
    cli 
    in al, 0x92
    or al, 2
    out 0x92, al

    lgdt [gdtDescriptor]

    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ;jmp 0x08:protectedMode

bits 32
protectedMode:
    mov ax, 0x10
    mov ds, ax
    mov es, ax  
    

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