ENTRY(loader)

SECTIONS {
    . = 0x00100000;
    _kernel_start = .;

    .text ALIGN(4096) : {
        *(.text)
    }

    .rodata ALIGN(4096) : {
        *(.rodata)
    }

    .data ALIGN(4096) : {
        *(.data)
    }

    .bss ALIGN(4096) : {
        *(COMMON)
        *(.bss)
    }
    _kernel_end = .;
}
