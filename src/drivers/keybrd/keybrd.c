#include "../../include/keybrd/keybrd.h"
#include "../../include/idt.h"
#include "../../include/isr.h"
#include "../../include/typint.h"
#include "../../include/vga.h"
#include "../../include/io.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_INT 33 // IRQ1 + 32 (IDT offset)


static const char base_map[128] = {
	[0x02] = '1', [0x03] = '2', [0x04] = '3', [0x05] = '4',
	[0x06] = '5', [0x07] = '6', [0x08] = '7', [0x09] = '8',
	[0x0a] = '9', [0x0b] = '0', [0x0c] = '-', [0x0d] = '=',
	[0x0e] = '\b', [0x0f] = '\t',
	[0x10] = 'q', [0x11] = 'w', [0x12] = 'e', [0x13] = 'r',
	[0x14] = 't', [0x15] = 'y', [0x16] = 'u', [0x17] = 'i',
	[0x18] = 'o', [0x19] = 'p', [0x1a] = '[', [0x1b] = ']',
	[0x1c] = '\n',
	[0x1e] = 'a', [0x1f] = 's', [0x20] = 'd', [0x21] = 'f',
	[0x22] = 'g', [0x23] = 'h', [0x24] = 'j', [0x25] = 'k',
	[0x26] = 'l', [0x27] = ';', [0x28] = '\'', [0x29] = '`',
	[0x2b] = '\\',
	[0x2c] = 'z', [0x2d] = 'x', [0x2e] = 'c', [0x2f] = 'v',
	[0x30] = 'b', [0x31] = 'n', [0x32] = 'm', [0x33] = ',',
	[0x34] = '.', [0x35] = '/', [0x39] = ' ', [0x01] = '\x1b',
};

static const char shift_map[128] = {
	[0x02] = '!', [0x03] = '@', [0x04] = '#', [0x05] = '$',
	[0x06] = '%', [0x07] = '^', [0x08] = '&', [0x09] = '*',
	[0x0a] = '(', [0x0b] = ')', [0x0c] = '_', [0x0d] = '+',
	[0x0e] = '\b', [0x0f] = '\t',
	[0x10] = 'Q', [0x11] = 'W', [0x12] = 'E', [0x13] = 'R',
	[0x14] = 'T', [0x15] = 'Y', [0x16] = 'U', [0x17] = 'I',
	[0x18] = 'O', [0x19] = 'P', [0x1a] = '{', [0x1b] = '}',
	[0x1c] = '\n',
	[0x1e] = 'A', [0x1f] = 'S', [0x20] = 'D', [0x21] = 'F',
	[0x22] = 'G', [0x23] = 'H', [0x24] = 'J', [0x25] = 'K',
	[0x26] = 'L', [0x27] = ':', [0x28] = '"', [0x29] = '~',
	[0x2b] = '|',
	[0x2c] = 'Z', [0x2d] = 'X', [0x2e] = 'C', [0x2f] = 'V',
	[0x30] = 'B', [0x31] = 'N', [0x32] = 'M', [0x33] = '<',
	[0x34] = '>', [0x35] = '?', [0x39] = ' ', [0x01] = '\x1b',
};

static U8 shift_on = 0;

void keyboard_handler(registers_t *regs) {
	(void)regs;
	U8 sc = byte_i(KEYBOARD_DATA_PORT);

	if (sc == 0x2A || sc == 0x36) {
		shift_on = 1;
	} else if (sc == 0xAA || sc == 0xB6) {
		shift_on = 0;
	} else if (!(sc & 0x80)) {
		char c = shift_on ? shift_map[sc] : base_map[sc];
		if (c) kbd_putchar(c);
	}
}

void keyboard_init(void) {
	register_interrupt_handler(KEYBOARD_INT, keyboard_handler);

	U8 mask = byte_i(0x21);   // Master PIC data/mask port
	mask &= ~(1 << 1);       // remove IRQ1 mask
	byte_o(0x21, mask);
}
