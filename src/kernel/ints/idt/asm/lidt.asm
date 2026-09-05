bits 32

global lidt
lidt:
    push ebp
    mov ebp, esp

    mov eax, [ebp + 8]
    lidt [eax]
    
    pop ebp
    ret