#include "panic.h"
#include "vga.h"

void kernel_panic(const char* msg) {
    asm volatile("cli");
    putstr_color("\n[ KERNEL PANIC ] ", COLOR_LIGHT_RED);
    putstr(msg);
    putstr("\n");
    while (1) asm volatile("hlt");   //stop
}

