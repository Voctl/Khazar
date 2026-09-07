#include "commands.h"
#include "types.h"
#include "vga.h"

U0 speek(char *args)
{
    U32 addr;
    U8 *ptr;
    U8 value;

    if (!args || !*args) {
        return;
    }

    addr = strtohex(args);
    ptr = (U8 *)addr;
    value = *ptr;
    puthex(value);
}
