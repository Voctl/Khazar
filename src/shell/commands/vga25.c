#include "vga.h"
#include "types.h"

U0 vga25(char *args) {
    (U0)args;

    vga_set_80x25();
}
