ENTRY(loader)

SECTIONS {
    . = 0x00100000;
    _kernel_start = .;

    .text ALIGN(4096) : {
        *(.text)
    }

    .rodata ALIGN(4096) : {
        *(.rodata .rodata.*)
    }

    .data ALIGN(4096) : {
        *(.data)
    }

    .bss ALIGN(4096) : {
        _bss_start = .;
        *(COMMON)
        *(.bss)
        _bss_end = .;
    }
    _kernel_end = .;
    end = .;
}
