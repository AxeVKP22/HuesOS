OUTPUT_FORMAT(binary)

SECTIONS
{
    . = 0x7E00;

    .text :
    {
        *(.text)
    }

    .rodata :
    {
        *(.rodata)
    }

    .data :
    {
        *(.data)
    }

    .bss :
    {
        *(COMMON)
        *(.bss)
    }

    /DISCARD/ :
    {
        *(.eh_frame)
        *(.comment)
        *(.note.GNU-stack)
    }
}