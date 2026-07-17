#include "../../include/pit/pit.h"
#include "../../arch/x86_64/cpu/isr.h"
#include "types.h"
#include "../../include/vga.h"
#include "../../arch/x86_64/port_io.h"

U32 volatile tick = 0;
U32 timer_frequency = 0;

static void timer_callback(registers_t *regs) { tick++; }

void init_timer(U32 frequency) {
  timer_frequency = frequency;
  register_interrupt_handler(IRQ0, &timer_callback);

  U32 divisor = 1193180 / frequency;

  byte_o(0x43, 0x36);

  U8 l = (U8)(divisor & 0xFF);
  U8 h = (U8)((divisor >> 8) & 0xFF);
  byte_o(0x40, l);
  byte_o(0x40, h);
}

void sleep(U32 ms) {
  U32 start_tick = tick;
  U32 ticks_to_wait = (ms * timer_frequency) / 1000;
  while (tick - start_tick < ticks_to_wait)
    ;
}
