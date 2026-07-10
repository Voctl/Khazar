#ifndef KEYBRD_H
#define KEYBRD_H

#include "../isr.h"
#include "../typint.h"

void keyboard_handler(registers_t *regs) ;
void keyboard_init(void);

#endif
