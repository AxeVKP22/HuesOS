bits 16 
[org 0x7e00] 
    mov ah, 0x0e 
    mov al,'W' 
    int 10h 
    jmp $