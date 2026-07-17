#include "sound.h"
#include "types.h"
#include "commands.h"

U0 cmd_beep(char *args){
    (U0)args;
    beep();
}
