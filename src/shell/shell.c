#include "../include/shell.h"
#include "../include/typint.h"
#include "../include/vga.h"
#include "../include/sound.h"
#include "../include/pit/pit.h"
#include "../include/string.h"
#include "../include/keybrd/keybrd.h"
#include "../include/specialch/khazar.h"
#include "../include/specialch/fetch.h"

#define SHELL_INM 50
#define COMMAND_COUNT (sizeof(commands) / sizeof(commands[0]))


static U0 helpc(char *args){
    (U0)args;
    putstr((STR8_C)"\nhelp\n");
    putstr_color((STR8_C)"version\n", COLOR_LIGHT_BLUE);
    putstr_color((STR8_C)"fetch\n", COLOR_LIGHT_BLUE);
    putstr((STR8_C)"halt\n");
    putstr((STR8_C)"clear\n");
    putstr((STR8_C)"beep\n");
}

static U0 clearc(char *args){
    (U0)args;

    clear();
}

static U0 versionc(char *args){
    (U0)args;
    khazar();
    putstr((STR8_C)" - alpha\n");
}

static U0 haltc(char *args){
    (U0)args;

    putstr_color((STR8_C)"halting . . .", COLOR_RED);
    while (1){
        asm volatile("cli; hlt");
    }

}

static U0 beepc(char *args){

    (U0)args;

    beep();
}

static U0 fetchc(char *args){
    (U0)args;

    fetch();
}


static const struct shell_command commands[] = {
    {"help" , helpc },
    {"version", versionc},
    {"clear", clearc},
    {"halt", haltc},
    {"beep", beepc},
    {"fetch", fetchc}
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

    putstr_color((STR8_C)" [ ERROR ] ", COLOR_RED);
    putstr((STR8_C)"Unknown cmd");
}


U0 shell(U0){
    char inputbuff[SHELL_INM + 1];
    I32 lenofcm = 0;
    I32 offshit = 0;
    char key;
    I32 i;

    keyboard_init();
    clear();
    putstr((STR8_C)"Welcome to ");
    putstr_color((STR8_C)"KhazarOS\n", COLOR_GREEN);
    putstr((STR8_C)"Do u want to see commands, type ");
    putstr_color((STR8_C)"'help'\n", COLOR_RED);
    putstr_color((STR8_C)"~# ", COLOR_GREEN);

    while (1) {
        if (keyboard_poll_char(&key)) {
            if (key == '\n') {
                if (offshit < lenofcm) {
                    cursor_set(cursor_get() + (lenofcm - offshit) * 2);
                }
                kbd_putchar('\n');
                inputbuff[lenofcm] = '\0';

                execute_command(inputbuff);

                lenofcm = 0;
                offshit = 0;
                putstr((STR8_C)"\n");
                putstr_color((STR8_C)"~# ", COLOR_GREEN);
                continue;
            } else if (key == '\b') {
                if (offshit > 0) {
                    offshit--;
                    lenofcm--;

                    for (i = offshit; i < lenofcm; i++) {
                        inputbuff[i] = inputbuff[i + 1];
                    }
                    inputbuff[lenofcm] = '\0';

                    cursor_set(cursor_get() - 2);
                    for (i = offshit; i < lenofcm; i++) {
                        kbd_putchar(inputbuff[i]);
                    }
                    kbd_putchar(' ');
                    cursor_set(cursor_get() - (lenofcm - offshit + 1) * 2);
                }
            } else if (key == ARROW_KEY_LEFT) {
                if (offshit > 0) {
                    offshit--;
                    cursor_set(cursor_get() - 2);
                }
            } else if (key == ARROW_KEY_RIGHT) {
                if (offshit < lenofcm) {
                    offshit++;
                    cursor_set(cursor_get() + 2);
                }
            } else if (key == ARROW_KEY_UP || key == ARROW_KEY_DOWN) {
                continue;
            } else {
                if (lenofcm < SHELL_INM) {
                    for (i = lenofcm; i > offshit; i--) {
                        inputbuff[i] = inputbuff[i - 1];
                    }
                    inputbuff[offshit] = key;
                    lenofcm++;
                    inputbuff[lenofcm] = '\0';

                    for (i = offshit; i < lenofcm; i++) {
                        kbd_putchar(inputbuff[i]);
                    }

                    offshit++;
                    if (offshit < lenofcm) {
                        cursor_set(cursor_get() - (lenofcm - offshit) * 2);
                    }
                }
            }
        }
        asm volatile("pause");
    }
}
