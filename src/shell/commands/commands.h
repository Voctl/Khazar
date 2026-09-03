#pragma once
#include "types.h"
#include "shell.h"

U0 cmd_help(char *args);
U0 cmd_clear(char *args);
U0 cmd_version(char *args);
U0 cmd_halt(char *args);
U0 cmd_beep(char *args);
U0 cmd_fetch(char *args);
U0 vga50(char *args);
U0 vga25(char *args);
U0 speek(char *tokens[4], int count);


extern const struct shell_command commands[];
extern const size_t command_count;
