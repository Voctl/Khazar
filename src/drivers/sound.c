#include "../include/sound.h"
#include "../include/io.h"
#include "../include/typint.h"
#include "../include/pit/pit.h"

void nsound() {
    uint8_t tmp = byte_i(0x61) & 0xFC;
    byte_o(0x61, tmp);
}


void plays(uint32_t nfrequence) {
  uint32_t div;
  uint8_t tmp;

  div = 1193180 / nfrequence;
  byte_o(0x43, 0xb6);
  byte_o(0x42, (uint8_t)(div));
  byte_o(0x42, (uint8_t)(div >> 8));

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
