bits 16

extern sysOpen, sysClose, sysRead, sysWrite, sysNew

global syscallInit

syscallInit:
    push es

    xor ax, ax
    mov es, ax

    mov word [es:0x80 * 4], syscallEntry
    mov word [es:0x80 * 4 + 2], cs

    pop es
    ret

syscallEntry:

    pusha

    cmp ah, 0x00
    je .sysOpen
    cmp ah, 0x01
    je .sysClose
    cmp ah, 0x02
    je .sysRead
    cmp ah, 0x03
    je .sysWrite
    cmp ah, 0x04
    je .sysNew

    jmp .done

    .sysOpen:
        xor ah, ah
        push ax              ; flags
        push dx              ; filename
        call sysOpen
        add sp, 4

        mov [cs:syscall_result], ax
        jmp .done


    .sysClose:
        push ax              ; fd
        call sysClose
        add sp, 2

        mov [cs:syscall_result], ax
        jmp .done


    .sysRead:
        push dx              ; count
        push cx              ; buffer
        push ax              ; fd
        call sysRead
        add sp, 6

        mov [cs:syscall_result], ax
        jmp .done


    .sysWrite:
        push dx              ; count
        push cx              ; buffer
        push ax              ; fd
        call sysWrite
        add sp, 6

        mov [cs:syscall_result], ax
        jmp .done


    .sysNew:
        push dx              ; filename
        call sysNew
        add sp, 2

        mov [cs:syscall_result], ax
        jmp .done

    .done:
        popa
        mov ax, [cs:syscall_result]
        iret

syscall_result dw 0
