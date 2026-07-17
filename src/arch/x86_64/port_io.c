#include "types.h"
#include "../include/vga.h"
// prot byte input output

U8 byte_i(U16 port) {
  U8 result;
  __asm__ volatile(
      "in %%dx, %%al"
      : "=a"(result)
      : "d"(port)); // qaqas men burda gotururem port deyisenini DX registrine
                    // yukleyirem DXdeki unvani ALye yerlesdirirem ALdaki
                    // neticeni result deyisenine yaziram
  return result;
}

void byte_o(U16 port, U8 data) {
  __asm__ volatile(
      "out %%al, %%dx"
      :
      : "a"(data),
        "d"(port)); // qaqas men burda ise data deyisenini AL registrine
                    // yukleyrem port deyisenini DX registrine yukleyrem sonra
                    // da ALdaki bayti DX unvanina gonderrem
}
