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
void putstr_color(string_const string, uint8_t color);

// ekrana rengli yazi yazdirmaq da
void set_char_w_color(uint8_t character, uint8_t color, int32_t offset);

// kursorun hereketi
void cursor_set(int offset);
int cursor_get();

// memorye bit yazdirmaq
void set_char_in_memory(uint8_t character, int32_t offset);

// printf funksiyasi
void putstr(string_const string);
void puthex(uint64_t n);

// komekci functionlar
int32_t get_row(int32_t offset);
int32_t get_offset(int32_t col, int32_t row);
int32_t move_newl(int32_t offset);

void memorycpy(uint8_t *source, uint8_t *dest,
               int32_t nbyte); // newline ucun memorye copy edir bu function

int32_t scrolln(int32_t offset); // bu ise ekrani scroll edib yuxari kocurur

void putdec(uint64_t n); // ekrana reqem yazmaga komek edir
// ekrani clear edir
void clear();


