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


global inl
inl:
    push ebp
    mov ebp, esp

    mov dx, [ebp + 8]
    in eax, dx

    pop ebp
    ret

global outl
outl:
    push ebp
    mov ebp, esp

    mov dx, [ebp + 8]
    mov eax, [ebp + 12]
    out dx, eax

    pop ebp
    ret