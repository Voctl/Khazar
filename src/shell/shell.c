/* SPDX-License-Identifier: GPL-2.0-only */
/* Copyright (C) 2026 Vahid Khalafov and Denis Gulmammadov               */
/* Denis Gulmammadov - KhazarOS uyğunlaşdırması    */

#include "../include/shell.h"
#include "../include/vga.h"
#include "../include/io.h"
#include "../include/typint.h"
#include <stddef.h>
#include <stdbool.h>

static volatile uint8_t kb_buf[KB_BUFFER_SIZE];
static volatile int     kb_head = 0;  
static volatile int     kb_tail = 0;  

void keyboard_buffer_push(uint8_t c)
{
    int next = (kb_head + 1) % KB_BUFFER_SIZE;
    if (next != kb_tail) {   
        kb_buf[kb_head] = c;
        kb_head = next;
    }
}

int keyboard_poll_char(uint8_t *out)
{
    if (kb_head == kb_tail)
        return 0;
    *out  = kb_buf[kb_tail];
    kb_tail = (kb_tail + 1) % KB_BUFFER_SIZE;
    return 1;
}

//vga wrappers

void print_character(char c)
{
    char buf[2] = { c, '\0' };
    putstr(buf);
}

void print_string(const char *s)
{
    putstr(s);
}

//
void print_hex_byte(uint8_t b)
{
    string_const hex = "0123456789ABCDEF";
    char buf[3];
    buf[0] = hex[(b >> 4) & 0xF];
    buf[1] = hex[b & 0xF];
    buf[2] = '\0';
    putstr(buf);
}


void move_cursor(int delta)
{
    int offset = cursor_get();
    offset += delta * 2;
    cursor_set(offset);
}

//utility funcs
static int is_hex_char(char c)
{
    return (c >= '0' && c <= '9') ||
           (c >= 'a' && c <= 'f') ||
           (c >= 'A' && c <= 'F');
}

static uint8_t hex_nibble(char c)
{
    if (c >= '0' && c <= '9') return (uint8_t)(c - '0');
    if (c >= 'a' && c <= 'f') return (uint8_t)(c - 'a' + 10);
    if (c >= 'A' && c <= 'F') return (uint8_t)(c - 'A' + 10);
    return 0;
}

uint64_t string_to_hex(const char *s)
{
    uint64_t result = 0;
    if (!s) return 0;
    if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X'))
        s += 2;
    while (is_hex_char(*s)) {
        result = (result << 4) | hex_nibble(*s);
        s++;
    }
    return result;
}

void itoa(uint32_t n, char *buf)
{
    char tmp[11];
    int  i = 0, j = 0;
    if (n == 0) { buf[0] = '0'; buf[1] = '\0'; return; }
    while (n > 0) {
        tmp[i++] = (char)('0' + n % 10);
        n /= 10;
    }
    while (i > 0)
        buf[j++] = tmp[--i];
    buf[j] = '\0';
}

// Khazar-ın strcmp-i yoxdur | new func
int strcmp(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2)) { s1++; s2++; }
    return (unsigned char)*s1 - (unsigned char)*s2;
}

static uint32_t rand_state = 12345UL;

void random_seed(uint32_t s)
{
    rand_state = s;
}

/* Park-Miller LCG */
uint32_t random(void)
{
    rand_state = rand_state * 1664525UL + 1013904223UL;
    return rand_state;
}


//Commands

#define SHELL_INPUT_MAX 63

static void cmd_help(char *args)
{
    (void)args;
    putstr("help\nclear\nhalt\ncolor\npeek\n"
           "dump\nrandom\npoke");
}

static void cmd_clear(char *args)
{
    (void)args;
    clear();
}

static void cmd_halt(char *args)
{
    (void)args;
    putstr("Halting...\n");
    while (1) {
        asm volatile("cli; hlt");
    }
}

static void cmd_color(char *args)
{
    char    *fg_str;
    char    *bg_str = NULL;
    uint8_t  fg, bg, color;
    int      i;

    if (!args || !*args) {
        putstr("Usage: color <fg_hex> <bg_hex>\n");
        return;
    }
    fg_str = args;
    for (i = 0; args[i] != '\0'; i++) {
        if (args[i] == ' ') {
            args[i] = '\0';
            bg_str  = &args[i + 1];
            break;
        }
    }
    if (!bg_str || !*bg_str) {
        putstr("Usage: color <fg_hex> <bg_hex>\n");
        return;
    }
    fg    = (uint8_t)string_to_hex(fg_str);
    bg    = (uint8_t)string_to_hex(bg_str);
    color = (uint8_t)((bg << 4) | (fg & 0x0F));
    // TODO: global rengi saxla, putstr_color ilə istifadə et
    putstr("Color set to 0x");
    print_hex_byte(color);
    putstr("\n");
}

// peek <addr_hex> - 1 byte oxu 
static void cmd_peek(char *args)
{
    uint64_t  addr;
    uint8_t  *ptr;

    if (!args || !*args) {
        putstr("Usage: peek <addr_hex>\n");
        return;
    }
    addr  = string_to_hex(args);
    ptr   = (uint8_t *)addr;
    putstr("0x");
    print_hex_byte(*ptr);
    putstr("\n");
}

// poke <addr_hex> <val_hex> - 1 byte yaz 
static void cmd_poke(char *args)
{
    char    *addr_str;
    char    *val_str = NULL;
    uint64_t address;
    uint8_t  value, *ptr;
    int      i;

    if (!args || !*args) {
        putstr("Usage: poke <addr_hex> <val_hex>\n");
        return;
    }
    addr_str = args;
    for (i = 0; args[i] != '\0'; i++) {
        if (args[i] == ' ') {
            args[i] = '\0';
            val_str = &args[i + 1];
            break;
        }
    }
    if (!val_str || !*val_str) {
        putstr("Usage: poke <addr_hex> <val_hex>\n");
        return;
    }
    address = string_to_hex(addr_str);
    value   = (uint8_t)string_to_hex(val_str);
    ptr     = (uint8_t *)address;
    *ptr    = value;
    putstr("Wrote 0x");
    print_hex_byte(value);
    putstr(" -> 0x");
    puthex(address);
    putstr("\n");
}

// dump <addr_hex> - 16 byte göstər 
static void cmd_dump(char *args)
{
    uint64_t  addr;
    uint8_t  *ptr;
    int       i;

    if (!args || !*args) {
        putstr("Usage: dump <addr_hex>\n");
        return;
    }
    addr = string_to_hex(args);
    ptr  = (uint8_t *)addr;

    puthex(addr);
    putstr(":  ");
    for (i = 0; i < 16; i++) {
        print_hex_byte(ptr[i]);
        print_character(' ');
        if (i == 7) putstr("| ");
    }
    putstr("\n");
}


static void cmd_random(char *args)
{
    char buf[12];
    (void)args;
    itoa(random(), buf);
    putstr(buf);
    putstr("\n");
}

struct shell_command {
    const char  *name;
    void       (*handler)(char *args);
};

static const struct shell_command commands[] = {
    { "help",   cmd_help   },
    { "clear",  cmd_clear  },
    { "halt",   cmd_halt   },
    { "color",  cmd_color  },
    { "peek",   cmd_peek   },
    { "dump",   cmd_dump   },
    { "random", cmd_random },
    { "poke",   cmd_poke   }
};
#define COMMAND_COUNT (sizeof(commands) / sizeof(commands[0]))

static void execute_command(char *input)
{
    char   *args;
    size_t  i;

    if (*input == '\0') return;

    //
    args = input;
    while (*args != ' ' && *args != '\0') args++;
    if (*args == ' ') { *args = '\0'; args++; }

    for (i = 0; i < COMMAND_COUNT; i++) {
        if (strcmp(input, commands[i].name) == 0) {
            commands[i].handler(args);
            return;
        }
    }
    putstr("Unknown command: ");
    putstr(input);
    putstr("\n");
}

//main shell loop
void shell(void)
{
    char    input_buffer[SHELL_INPUT_MAX + 1];
    int     length       = 0;
    int     cursor_index = 0;
    uint8_t key;
    int     i;

    clear();
    putstr("Welcome to KhazarOS!\n");
    putstr("Type 'help' to see available commands.\n");
    print_string("# ");

    while (1) {
        if (!keyboard_poll_char(&key)) {
            asm volatile("pause");
            continue;
        }

        if (key == '\n') {
            // cursoru tulla basa
            if (cursor_index < length)
                move_cursor(length - cursor_index);
            print_character('\n');
            input_buffer[length] = '\0';
            execute_command(input_buffer);
            length       = 0;
            cursor_index = 0;
            print_string("\n# ");

        } else if (key == '\b') {
            if (cursor_index > 0) {
                cursor_index--;
                length--;
                for (i = cursor_index; i < length; i++)
                    input_buffer[i] = input_buffer[i + 1];
                input_buffer[length] = '\0';
                move_cursor(-1);
                for (i = cursor_index; i < length; i++)
                    print_character(input_buffer[i]);
                print_character(' ');
                move_cursor(-(length - cursor_index + 1));
            }

        } else if (key == ARROW_KEY_LEFT) {
            if (cursor_index > 0) {
                cursor_index--;
                move_cursor(-1);
            }

        } else if (key == ARROW_KEY_RIGHT) {
            if (cursor_index < length) {
                cursor_index++;
                move_cursor(1);
            }

        } else if (key == ARROW_KEY_UP || key == ARROW_KEY_DOWN) {
            continue;

        } else {
            if (length < SHELL_INPUT_MAX) {
                // cursor movq char yerlesir
                for (i = length; i > cursor_index; i--)
                    input_buffer[i] = input_buffer[i - 1];
                input_buffer[cursor_index] = (char)key;
                length++;
                input_buffer[length] = '\0';
                for (i = cursor_index; i < length; i++)
                    print_character(input_buffer[i]);
                cursor_index++;
                if (cursor_index < length)
                    move_cursor(-(length - cursor_index));
            }
        }
    }
}
