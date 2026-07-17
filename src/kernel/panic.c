#include "panic.h"
#include "vga.h"

void kernel_panic(const char* msg) {
    asm volatile("cli");
    putstr_color((STR8_C)"\n[ KERNEL PANIC ] ", COLOR_LIGHT_RED);
    putstr((STR8_C)msg);
    putstr((STR8_C)"\n");
    while (1) asm volatile("hlt");   //stop
}

