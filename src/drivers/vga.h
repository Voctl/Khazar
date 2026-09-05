#pragma once

#define COLOR_BLACK     0x00
#define COLOR_BLUE      0x01
#define COLOR_GREEN     0x02
#define COLOR_CYAN      0x03
#define COLOR_RED       0x04
#define COLOR_MAGENTA   0x05
#define COLOR_BROWN     0x06
#define COLOR_LIGHT_GREY 0x07
#define COLOR_DARK_GREY  0x08
#define COLOR_LIGHT_BLUE 0x09
#define COLOR_LIGHT_GREEN 0x0A
#define COLOR_LIGHT_CYAN 0x0B
#define COLOR_LIGHT_RED  0x0C
#define COLOR_LIGHT_MAGENTA 0x0D
#define COLOR_YELLOW    0x0E
#define COLOR_WHITE     0x0F

#include "types.h"

// puststr but colorized version
void putstr_color(STR8_C string, U8 color);

// setting colorized char into somewhere in display
void set_char_w_color(U8 character, U8 color, I32 offset);

// cursor shits
void cursor_set(int offset);
int cursor_get();

// set_char_w_color but no color
void set_char_in_memory(U8 character, I32 offset);

// prints
void putstr(STR8_C string);
void puthex(U64 n);

// helper
I32 get_row(I32 offset);
I32 get_offset(I32 col, I32 row);
I32 move_newl(I32 offset);

void memorycpy(U8 *source, U8 *dest,
               I32 nbyte); // memory copy func for newline

I32 scrolln(I32 offset); // scrooll

void putdec(U64 n); // printing decimal number

void clear(); // clean display

U0 kbd_putchar(char c);

// VGA text mode switching -- 80x25 (default) ve 80x50 (8x8 font)
extern I32 VGA_ROWS;
extern I32 VGA_COLS;
void vga_set_80x25();
void vga_set_80x50();
