#ifndef KEYBRD_H
#define KEYBRD_H

#include "../arch/x86_64/cpu/isr.h"
#include "types.h"

#define ARROW_KEY_UP    ((char)0x01)
#define ARROW_KEY_DOWN  ((char)0x02)
#define ARROW_KEY_LEFT  ((char)0x03)
#define ARROW_KEY_RIGHT ((char)0x04)

void keyboard_handler(registers_t *regs);
void keyboard_init(void);
BOOL keyboard_poll_char(char *out);

#endif
