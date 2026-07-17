#include "vga.h"
#include "types.h"
#include "commands.h"

U0 cmd_help(char *args){
    (U0)args;
    putstr((STR8_C)"\nhelp\n");
    putstr_color((STR8_C)"version\n", COLOR_LIGHT_BLUE);
    putstr_color((STR8_C)"fetch\n", COLOR_LIGHT_BLUE);
    putstr((STR8_C)"halt\n");
    putstr((STR8_C)"clear\n");
    putstr((STR8_C)"beep\n");
}
