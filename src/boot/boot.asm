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

    ; print S
    mov ah, 0x0e
    mov al, 'S'     ; this code is so unstable that without printring s kernel will not load :/
    int 0x10

    mov al, [boot_drive] ; for some reason, without this kernel will not load
    call print_hex       ; and this too

    ; read sector 2
    mov ah, 0x02
    mov al, 8          ; sectors to read
    mov ch, 0          ; cylinder
    mov cl, 3          ; sector (starts at 1)
    mov dh, 0          ; head
    mov dl, [boot_drive]

    xor bx, bx
    mov es, bx
    mov bx, 0x7e00     ; load address

    int 0x13
    jc disk_error

    ; success
    xchg bx,bx
    mov dl, [boot_drive]
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

;cat build/kernel.map
;ndisasm -b 16 -o 0x7C00 build/os.img
;qemu-system-i386 -drive format=raw,file=build/os.img
;ndisasm -b 16 build/os.img 
;dd if=/home/axe/gitProjects/wowos/build/os.img of=/dev/sda bs=4M status=progress conv=fsync