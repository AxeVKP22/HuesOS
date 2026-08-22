bits 32

; al - return value
global inb
inb:
    push ebp
    mov ebp, esp

    mov dx, [ebp + 8]
    in al, dx

    pop ebp
    ret

global outb
outb:
    push ebp
    mov ebp, esp

    mov dx, [ebp + 8]
    mov al, [ebp + 12]
    out dx, al

    pop ebp
    ret
    