#include "../include/shell.h"
#include "../include/typint.h"
#include "../include/vga.h"
#include "../include/sound.h"
#include "../include/pit/pit.h"
#include "../include/string.h"

#define SHELL_INM 50
#define COMMAND_COUNT (sizeof(commands) / sizeof(commands[0]))


static U0 helpc(char *args){
    (U0)args;
    putstr("\nhelp\n");
    putstr("version\n");
    putstr("halt\n");
    putstr("clear\n");
    putstr("beep\n");
}

static U0 clearc(char *args){
    (U0)args;

    clear();
}

static U0 versionc(char *args){
    (U0)args;
    putstr("KhazarOS - alpha");
}

static U0 haltc(char *args){
    (U0)args;

    while (1){
        asm volatile("cli; hlt");
    }

}

static U0 beepc(char *args){

    (U0)args;

    beep();
}


static const struct shell_command commands[] = {
    {"help" , helpc },
    {"version", versionc},
    {"clear", clearc},
    {"halt", haltc},
    {"beep", beepc}
};


static void execute_command(char *input)
{
    char *args;
    size_t i;

    if (*input == '\0') {
        return;
    }

    args = input;

    while (*args != ' ' && *args != '\0') {
        args++;
    }

    if (*args == ' ') {
        *args = '\0';
        args++;
    }

    for (i = 0; i < COMMAND_COUNT; i++) {
        if (strcmp(input, commands[i].name) == 0) {
            commands[i].handler(args);
            return;
        }
    }

    putstr("Unknown cmd");

}
