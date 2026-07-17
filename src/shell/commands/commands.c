#include "commands.h"

const struct shell_command commands[] = {
    {"help",    cmd_help},
    {"version", cmd_version},
    {"clear",   cmd_clear},
    {"halt",    cmd_halt},
    {"beep",    cmd_beep},
    {"fetch",   cmd_fetch},
};

const size_t command_count =
    sizeof(commands) / sizeof(commands[0]);
