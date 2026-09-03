#include "commands.h"
#include "types.h"
#include "vga.h"

U0 speek(char *tokens[4], int count)
{
    if (count < 2) {
        putstr_color("[Usage]:", COLOR_RED);
        putstr( "PEEK <addr>\n");
        return;
    }
    U32 addr;
    U8 val = *(volatile U8 *)addr;
    putstr("[");
    puthex(addr);
    putstr("] = ");
    puthex((U32)val);
    putstr(" (");
    putdec((U32)val);
    putstr(")\n");
}
