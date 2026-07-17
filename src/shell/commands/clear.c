#include "vga.h"
#include "types.h"
#include "commands.h"

U0 cmd_clear(char *args){
    (U0)args;
    clear();
}
