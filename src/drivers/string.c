#include "../include/string.h"
#include "../include/memory/pmm.h"
#include "../include/typint.h"
#include "../include/vga.h"

void *memset(void *ptr, int value, size_t num) {
  U8 *p = ptr;

  while (num--)
    *p++ = (U8)value;

  return ptr;
}

void *memcpy(void *dest, const void *src, size_t num) {
  U8 *d = dest;
  const U8 *s = src;

  while (num--)
    *d++ = *s++;

  return dest;
}

char *strcpy(char *dest, const char *src) {
    char *ptr = dest;
    while (*src != '\0') {
        *dest++ = *src++;
    }
    *dest = '\0'; // NULL terminator
    return ptr;
}
