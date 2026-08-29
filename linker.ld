ENTRY(kmain)

SECTIONS
{
    . = 0x00100000;

    .text : {
        *(.text.kmain)
        *(.text*)
    }

    .rodata : {
        *(.rodata*)
    }

    .data : {
        *(.data*)
    }

    .bss : {
        *(COMMON)
        *(.bss*)
    }
}