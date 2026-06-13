ENTRY(kmain)

SECTIONS
{
    . = 0xffffffff80100000;

    .text : {
        *(.text*)
    }

    .rodata : {
        *(.rodata*)
    }

    .data : {
        *(.data*)
        KEEP(*(.limine_reqs*))
    }

    .bss : {
        *(.bss*)
    }
}
