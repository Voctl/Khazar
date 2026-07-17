#include "vga.h"
#include "types.h"
#include "commands.h"

static U0 khazar(U0){
    putstr_color((STR8_C)"KhazarOS", COLOR_GREEN);
}

U0 cmd_version(char *args){
    (U0)args;
    khazar();
    putstr((STR8_C)" - alpha\n");
}
