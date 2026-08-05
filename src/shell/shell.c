#include "../include/shell.h"
#include "types.h"
#include "vga.h"
#include "sound.h"

#include "pit.h"
#include "string.h"
#include "keyboard.h"
#include "commands/commands.h"

#define SHELL_INM 256

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

    for (i = 0; i < command_count; i++) {
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
