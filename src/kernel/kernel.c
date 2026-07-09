#include "../include/io.h"
#include "../include/sound.h"
#include "../include/gdt.h"
#include "../include/idt.h"
#include "../include/keybrd/keybrd.h"
#include "../include/memory/multiboot.h"
#include "../include/pit/pit.h"
#include "../include/typint.h"
#include "../include/vga.h"
#include "panic.h"
#include "../include/memory/pmm.h"
#include "../include/string.h"
#include "../include/memory/kheap.h"

void kernel_main(U64 multiboot_addr) {
  clear();
  init_gdt();
  idt_init();
  init_keyboard();
  init_timer(100);

  asm volatile("sti"); // interruptin ise dusmesi ucun
  putstr_color("[ INFO ]", COLOR_LIGHT_GREEN);
  putstr(" GDT Initialized\n");
  sleep(150);
  putstr_color("[ INFO ]", COLOR_LIGHT_GREEN);
  putstr(" IDT Initialized\n");
  sleep(150);
  putstr_color("[ INFO ]", COLOR_LIGHT_GREEN);
  putstr(" ISRs Initialized\n");
  sleep(150);
  putstr_color("[ INFO ]", COLOR_LIGHT_GREEN);
  putstr(" IRQ1 [keyboard] Initialized\n");
  sleep(150);
  multiboot_info_t *mb =
    (multiboot_info_t*)multiboot_addr;
  pmm_init(mb);
  putstr_color("[ INFO ]", COLOR_LIGHT_GREEN);
  putstr(" PMM initialized\n");
//beep
  U32 hz = 100;
  beep();
  plays(hz);
  sleep(150);
  nsound();
  sleep(700);
  clear();
  const char *logo = "KhazarOS";
  I32 offset = get_offset(35, 11);
  for (int i = 0; logo[i] != '\0'; i++) {
    sleep(100);
    set_char_w_color(logo[i], COLOR_LIGHT_GREEN, offset);
    offset += 2;
  }
  // :D emoji
  I32 offsheet = get_offset(39, 12);
  set_char_w_color((U8)0x01, COLOR_LIGHT_GREEN, offsheet);

  // qardas bu memorydir PMM ucun lazm olcaq
  if (mb->flags & MULTIBOOT_FLAG_MAP) {
    putstr("\n[ MEMORY ]\n");
    sleep(300);
    multiboot_entry_t *entry = (multiboot_entry_t *)((U64)mb->mmap_addr);
    U64 end = (U64)mb->mmap_addr + mb->mmap_length;
    // print
    while ((U64)entry < end) {
      if (entry->type == 1) {
        putstr("[ USABLE ]   addr: ");
        puthex(entry->addr);
        putstr("  len: ");
        puthex(entry->len);
        putstr("\n");

      } else {
        putstr("[ RESERVED ] addr: ");
        puthex(entry->addr);
        putstr("  len: ");
        puthex(entry->len);
        putstr("\n");
      }
      entry = (multiboot_entry_t *)((U64)entry + entry->size + 4);
    }
  }
  sleep(600);
  clear();
  pmm_stats();
  beep();
  while (1)
    ;
}
