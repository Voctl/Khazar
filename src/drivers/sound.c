#include "../include/sound.h"
#include "../arch/x86_64/port_io.h"
#include "types.h"
#include "../include/pit/pit.h"

void nsound() {
    U8 tmp = byte_i(0x61) & 0xFC;
    byte_o(0x61, tmp);
}


void plays(U32 nfrequence) {
  U32 div;
  U8 tmp;

  div = 1193180 / nfrequence;
  byte_o(0x43, 0xb6);
  byte_o(0x42, (U8)(div));
  byte_o(0x42, (U8)(div >> 8));

  tmp = byte_i(0x61);
  if (tmp != (tmp | 3)){
      byte_o(0x61, tmp | 3);
  }
}

void beep(){
    plays(1000);
    sleep(100);
    nsound();
}
