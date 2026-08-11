#include "../arch/x86_64/port_io.h"
#include "sound.h"
#include "../arch/x86_64/cpu/gdt.h"
#include "../arch/x86_64/cpu/idt.h"
#include "keyboard.h"
#include "../include/multiboot.h"
#include "pit.h"
#include "types.h"
#include "vga.h"
#include "panic.h"
#include "pmm.h"
#include "string.h"
#include "kheap.h"
#include "../include/shell.h"
#include "../graphics/frambuffer.h"
#include "proc/process.h"


extern U8 end; /* the symbol defined in link.ld
                * as the end of the kernel as "end = ."*/

/* [ TEST ] TASKS TEST IMPLEMENTATION */
void task_a(void) {
    while (1) {
        putstr_color((STR8_C)"[TASK A] running\n", COLOR_LIGHT_CYAN);
        sleep(50);
        yield(); // next task
    }
}

void task_b(void) {
    while (1) {
        putstr_color((STR8_C)"[TASK B] running...\n", COLOR_LIGHT_MAGENTA);
        sleep(50);
        yield(); // növbəni A-ya ötürür
    }
}
/* [ TEST ] TASKS TEST IMPLEMENTATION */



void kernel_main(U64 multiboot_addr) {
  clear();
  init_gdt();
  idt_init();
  init_timer(100);
  keyboard_init();

  asm volatile("sti"); // for start the interrupt
  putstr_color((STR8_C)"[ INFO ]", COLOR_LIGHT_GREEN);
  putstr((STR8_C)" GDT Initialized\n");
  sleep(150);
  putstr_color((STR8_C)"[ INFO ]", COLOR_LIGHT_GREEN);
  putstr((STR8_C)" IDT Initialized\n");
  sleep(150);
  putstr_color((STR8_C)"[ INFO ]", COLOR_LIGHT_GREEN);
  putstr((STR8_C)" ISRs Initialized\n");
  sleep(150);
  putstr_color((STR8_C)"[ INFO ]", COLOR_LIGHT_GREEN);
  putstr((STR8_C)" IRQ1 [keyboard] Initialized\n");
  sleep(150);
  multiboot2_info_t *mbi = (multiboot2_info_t*)multiboot_addr;
  multiboot2_tag_mmap_t *mmap_tag = 0;

  U64 ptr = (U64)mbi + sizeof(multiboot2_info_t);
  U64 ends = (U64)mbi + mbi->total_size;
  while (ptr < ends) {
    multiboot2_tag_t *tag = (multiboot2_tag_t*)ptr;
    if (tag->type == 0) break;
    if (tag->type == MULTIBOOT2_TAG_MMAP) {
      mmap_tag = (multiboot2_tag_mmap_t*)tag;
      pmm_init(mmap_tag);
    }
    if (tag->type == MULTIBOOT2_TAG_FRAMEBUFFER){
        testFrameb((multiboot2_tag_framebuffer_t*)tag);
    }
    ptr = (ptr + tag->size + 7) & ~7ULL;
  }
  putstr_color((STR8_C)"[ INFO ]", COLOR_LIGHT_GREEN);
  putstr((STR8_C)" PMM initialized\n");
  sleep(150);

  // we reserve the end of the kernel as the heap start, rounded down to 4KiB
  U64 heap_start = ((U64)&end + 0xFFF) & ~0xFFFUL;
  U64 heap_initial_size = 0x400000;      // 4 MiB - basliyic olcu
  U64 heap_max_size     = 0x2000000;     // 32 MiB - boyuye bilecek maksimum

  // we mark all frames in this range as "used" in PMM, so they are not available to the allocator
  for (U64 addr = heap_start; addr < heap_start + heap_max_size; addr += 0x1000) {
      pmm_set_addr(addr);
  }
  // initializing heap
  init_heap(heap_start, heap_start + heap_initial_size, heap_max_size, 1, 0);
  putstr_color((STR8_C)"[ INFO ]", COLOR_LIGHT_GREEN);
  putstr((STR8_C)" Heap Initialized\n");
  sleep(500);
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
  if (mmap_tag) {
    putstr((STR8_C)"\n[ MEMORY ]\n");
    sleep(300);
    multiboot2_mmap_entry_t *entry = (multiboot2_mmap_entry_t*)((U64)mmap_tag + sizeof(multiboot2_tag_mmap_t));
    U64 mmap_end = (U64)mmap_tag + mmap_tag->size;
    while ((U64)entry < mmap_end) {
      if (entry->type == 1) {
        putstr((STR8_C)"[ USABLE ]   addr: ");
        puthex(entry->addr);
        putstr((STR8_C)"  len: ");
        puthex(entry->len);
        putstr((STR8_C)"\n");

      } else {
        putstr((STR8_C)"[ RESERVED ] addr: ");
        puthex(entry->addr);
        putstr((STR8_C)"  len: ");
        puthex(entry->len);
        putstr((STR8_C)"\n");
      }
      entry = (multiboot2_mmap_entry_t*)((U64)entry + mmap_tag->entry_size);
    }
  }
  pmm_stats();
  beep();


  /* [ TEST ]  TASK IMPLEMENTATION */
  proc_init(); // init procs array
    createp((U64)task_a);
    createp((U64)task_b);
    putstr_color((STR8_C)"[ INFO ] Starting Multitasking Scheduler\n", COLOR_LIGHT_GREEN);
    sleep(300);
    yield(); // jump to first process (task_a)
    // yield() called, control is in task_a/task_b loop now.
    // code wont reach shell() unless tasks exit shi.
  sleep(1000);
  shell();
}
