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

#include "typint.h"

//rengli putstr
void putstr_color(STR8_C string, U8 color);

// ekrana rengli yazi yazdirmaq da
void set_char_w_color(U8 character, U8 color, I32 offset);

// kursorun hereketi
void cursor_set(int offset);
int cursor_get();

// memorye bit yazdirmaq
void set_char_in_memory(U8 character, I32 offset);

// printf funksiyasi
void putstr(STR8_C string);
void puthex(U64 n);

// komekci functionlar
I32 get_row(I32 offset);
I32 get_offset(I32 col, I32 row);
I32 move_newl(I32 offset);

void memorycpy(U8 *source, U8 *dest,
               I32 nbyte); // newline ucun memorye copy edir bu function

I32 scrolln(I32 offset); // bu ise ekrani scroll edib yuxari kocurur

void putdec(U64 n); // ekrana reqem yazmaga komek edir
// ekrani clear edir
void clear();

U0 kbd_putchar(char c);
