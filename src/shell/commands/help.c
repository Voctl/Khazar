#include "commands.h"
#include "types.h"
#include "vga.h"

U0 cmd_help(char *args) {
  (U0) args;
  putstr_color((STR8_C) "\nhelp  - LISTING FUNCTIONS\n", COLOR_RED);
  putstr_color((STR8_C) "version - SHOWS THE VERSION OF KERNEL\n", COLOR_LIGHT_BLUE);
  putstr_color((STR8_C) "fetch   - MINIMAL FETCH APP\n", COLOR_LIGHT_BLUE);
  putstr_color((STR8_C) "halt    - HALTING KERNEL\n", COLOR_WHITE);
  putstr_color((STR8_C) "clear   - CLEAR THE SHELL\n", COLOR_WHITE);
  putstr_color((STR8_C) "beep    - BEEP ?\n", COLOR_WHITE);
  putstr_color((STR8_C) "vga50   - CHANGING FONT TO 80X50\n", COLOR_WHITE);
  putstr_color((STR8_C) "vga25   - CHANGING FONT TO 80X25\n", COLOR_WHITE);
  putstr_color((STR8_C) "peek   - READ A BYTE FROM MEMORY\n", COLOR_WHITE);
}
