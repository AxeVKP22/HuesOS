bits 16 
[org 0x7e00] 

    call clear_screen

    mov si, hello
    call print_string
    jmp $


print_string:
    lodsb
    cmp al, 0
    je .done
    mov ah, 0x0e
    int 0x10
    jmp print_string
.done:
    ret

new_line:
    mov al, 0x0d
    mov ah, 0x0e
    int 10h

    mov al, 0x0a
    mov ah, 0x0e
    int 10h
    ret

clear_screen:
    mov ah, 0x00
    mov al, 0x03
    int 0x10
    ret



hello db 'Hello, World!', 0