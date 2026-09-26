#include "commands.h"
#include "types.h"
#include "vga.h"
#include "../../fs/fat16/fat16.h"

U0 cmd_dir(char *args)
{
    (U0)args;
    fat16_list();
}
