#ifndef PORT_IO_H
#define PORT_IO_H

#include "types.h"

// prot byte input output

U8 byte_i(U16 port);

void byte_o(U16 port, U8 data);

// took from CDos-x86
static inline U0 io_wait(U0)
{
    /* write to an unused port -- gives the PIC/hardware time to catch up */
    __asm__ volatile ("outb %%al, $0x80" : : "a"(0));
}

#endif /* PORT_IO_H */
