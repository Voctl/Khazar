#include "../../include/keybrd/keybrd.h"
#include "../../include/idt.h"
#include "../../include/isr.h"
#include "../../include/typint.h"
#include "../../include/vga.h"

#define IRQ1 33

static void keyboard_callback(registers_t *regs) {
  uint8_t scancode = byte_i(0x60);
  putstr("\n");
  byte_o(0x20, 0x20);
}

void init_keyboard() { register_interrupt_handler(IRQ1, keyboard_callback); }
