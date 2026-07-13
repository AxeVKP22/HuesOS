bits 16
[org 0x7c00]

start:
    mov [boot_drive], dl

    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00
    sti

    ; set video mode 03h
    mov ax, 0x0003
    int 0x10

    mov al, [boot_drive] ; for some reason, without this kernel will not load
    call print_hex       ; and this too

    ; read sector 2
    mov ah, 0x02
    mov al, 2          ; sectors to read
    mov ch, 0          ; cylinder
    mov cl, 2          ; sector (starts at 1)
    mov dh, 0          ; head
    mov dl, [boot_drive]

    xor bx, bx
    mov es, bx
    mov bx, 0x7e00     ; load address

    int 0x13
    jc disk_error

    ; success
    xchg bx,bx
    jmp 0x0000:0x7e00

disk_error:
    mov [error_code], ah

    mov al, [error_code]
    call print_hex

    cli
    hlt

; print byte in AL as hex
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

boot_drive db 0
error_code db 0


times 510-($-$$) db 0
dw 0xaa55

;ndisasm -b 16 -o 0x7C00 build/os.img
;bochs -f bochsrc.txt 
;bochs -debugger -f bochsrc.txt ;ndisasm -b 16 os.img 
;dd if=/home/axe/gitProjects/wowos/build/os.img of=/dev/sda bs=4M status=progress conv=fsync