#include "string.h"
#include "pmm.h"
#include "types.h"
#include "vga.h"

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

I32 strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

/*There is only 4 functions in here right now :
 * (memset, memcpy, strcpy, strcmp).*/

// TODO : memmove, strlen, strncmp, strchr, strstr, strtol/atoi, qsort
