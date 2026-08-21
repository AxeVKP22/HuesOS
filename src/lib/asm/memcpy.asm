bits 32

global memcpy
memcpy:
    push ebp
    mov ebp, esp

    push esi
    push edi

    mov esi, [ebp + 8]
    mov edi, [ebp + 12]
    mov ecx, [ebp + 16]

    cld
    rep movsb

    pop edi
    pop esi

    pop ebp
    ret