#include "../../include/keybrd/keybrd.h"
#include "../../include/idt.h"
#include "../../include/isr.h"
#include "../../include/typint.h"
#include "../../include/vga.h"
#include "../../include/io.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_INT 33 // IRQ1 + 32 (IDT offset)

static const char kbd_map[] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, 0, 0, ' '
};

void keyboard_handler() {
    U8 scancode = byte_i(KEYBOARD_DATA_PORT);

    if (!(scancode & 0x80)) {
        if (scancode < sizeof(kbd_map)) {
            char key = kbd_map[scancode];
            if (key != 0) {
            }
        }
    }

    byte_o(0x20, 0x20);
}
