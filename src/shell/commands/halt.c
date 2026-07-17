#include "vga.h"
#include "types.h"
#include "commands.h"

U0 cmd_halt(char *args){
    (U0)args;
    putstr_color((STR8_C)"halting . . .", COLOR_RED);
    while (1){
        asm volatile("cli; hlt");
    }
}
