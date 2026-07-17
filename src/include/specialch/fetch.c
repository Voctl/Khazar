#include "../vga.h"
#include "../typint.h"
#include "../memory/pmm.h"
#include "fetch.h"

static const U8 *logo_lines[] = {
    (STR8_C)"        ____",
    (STR8_C)"      /      \\",
    (STR8_C)"     /   ____/",
    (STR8_C)"    /   /",
    (STR8_C)"   |   |",
    (STR8_C)"   |   |__________",
    (STR8_C)"   \\              \\",
    (STR8_C)"    \\______________\\",
};

#define LOGO_LINES 8
#define LOGO_WIDTH 20
#define SEP_CHAR 0xB3

static void write_at(const U8 *str, U8 color, I32 col, I32 row) {
    I32 offset = get_offset(col, row);
    for (I32 i = 0; str[i] != '\0'; i++) {
        set_char_w_color(str[i], color, offset);
        offset += 2;
    }
}

static void fill_at(U8 ch, U8 color, I32 col, I32 row, I32 count) {
    I32 offset = get_offset(col, row);
    for (I32 i = 0; i < count; i++) {
        set_char_w_color(ch, color, offset);
        offset += 2;
    }
}

static I32 dec_len(U64 n) {
    if (n == 0) return 1;
    I32 len = 0;
    while (n > 0) { n /= 10; len++; }
    return len;
}

static void write_num_at(U64 n, U8 color, I32 col, I32 row) {
    U8 buf[12];
    I32 len = dec_len(n);
    buf[len] = '\0';
    if (n == 0) {
        buf[0] = '0';
        len = 1;
    } else {
        I32 i = len - 1;
        U64 m = n;
        while (m > 0) {
            buf[i--] = '0' + (m % 10);
            m /= 10;
        }
    }
    I32 offset = get_offset(col, row);
    for (I32 i = 0; i < len; i++) {
        set_char_w_color(buf[i], color, offset);
        offset += 2;
    }
}

U0 fetch(U0) {
    I32 cur_offset = cursor_get();
    I32 cur_row = get_row(cur_offset);
    I32 base_row = (cur_row > 0) ? cur_row : 0;

    I32 start_row = base_row + 1;
    I32 logo_col = 2;
    I32 info_col = 28;
    I32 i;

    write_at((STR8_C)"  KhazarOS", COLOR_LIGHT_GREEN, 0, base_row);

    for (i = 0; i < LOGO_LINES; i++) {
        write_at(logo_lines[i], COLOR_WHITE, logo_col, start_row + i);
    }

    for (i = 0; i < LOGO_LINES; i++) {
        set_char_w_color(SEP_CHAR, COLOR_DARK_GREY, get_offset(info_col - 2, start_row + i));
    }

    U64 total_mb     = total_memory / 1024 / 1024;
    U64 allocated_mb = allocated_memory / 1024 / 1024;
    U64 free_mb = (total_memory > allocated_memory)
                   ? (total_memory - allocated_memory) / 1024 / 1024 : 0;

    I32 row = start_row + 1;

    write_at((STR8_C)"OS",         COLOR_DARK_GREY,  info_col, row);
    write_at((STR8_C)"KhazarOS",   COLOR_WHITE,      info_col + 9, row);

    write_at((STR8_C)"Kernel",     COLOR_DARK_GREY,  info_col, row + 1);
    write_at((STR8_C)"alpha",      COLOR_WHITE,      info_col + 9, row + 1);

    write_at((STR8_C)"Shell",      COLOR_DARK_GREY,  info_col, row + 2);
    write_at((STR8_C)"khazar",     COLOR_WHITE,      info_col + 9, row + 2);

    write_at((STR8_C)"RAM",        COLOR_DARK_GREY,  info_col, row + 4);
    write_num_at(total_mb, COLOR_WHITE, info_col + 9, row + 4);
    write_at((STR8_C)"MiB",        COLOR_DARK_GREY,  info_col + 9 + dec_len(total_mb) + 1, row + 4);

    write_at((STR8_C)"Free",       COLOR_DARK_GREY,  info_col, row + 5);
    write_num_at(free_mb, COLOR_WHITE, info_col + 9, row + 5);
    write_at((STR8_C)"MiB",        COLOR_DARK_GREY,  info_col + 9 + dec_len(free_mb) + 1, row + 5);

    fill_at(0xCD, COLOR_DARK_GREY, info_col, row + 7, 30);

    write_at((STR8_C)"Packages",   COLOR_DARK_GREY,  info_col, row + 8);
    write_at((STR8_C)"0 (built-in)", COLOR_WHITE,    info_col + 9, row + 8);

    I32 end_row = start_row + LOGO_LINES + 1;
    fill_at(0xCD, COLOR_DARK_GREY, 0, end_row, 80);
    cursor_set(get_offset(0, end_row + 1));
}
