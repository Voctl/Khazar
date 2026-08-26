#include "string.h"
#include "pmm.h"
#include "types.h"
#include "vga.h"


/* Fills memory block with a byte value */
void *memset(void *ptr, int value, size_t num) {
  U8 *p = ptr;
  while (num--)
    *p++ = (U8)value;
  return ptr;
}

/* Copies non-overlapping memory area */
void *memcpy(void *dest, const void *src, size_t num) {
  U8 *d = dest;
  const U8 *s = src;
  while (num--)
    *d++ = *s++;

  return dest;
}

/* Copies null-terminated string */
char *strcpy(char *dest, const char *src) {
    char *ptr = dest;
    while (*src != '\0') {
        *dest++ = *src++;
    }
    *dest = '\0'; // NULL terminator
    return ptr;
}

/* Compares two strings byte-by-byte */
I32 strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

/*Copy memory area, vro its have one difference with that memcpy shit
 *u can see it if the source < destination its just reverse copying
 *the memory */
void *memmove(void *dest, const void *src, size_t num){
  U8 *d = dest;
  const U8 *s = src;
  if (d < s) return memcpy(dest, src, num);
  while (num--){
      d[num] = s[num];
  }
  return dest;
}

/*strlen u know bro just for take the length of a string |
 * Returns the legth of NULL terminated string*/
size_t strlen(const char *s) {
    const char *p = s;
    while (*p) {
        p++;
    }
    return (size_t)(p - s);
}

/*so, its doing wha : its just compare the part of string (
 * as a number) which is given as `size_t n`
 * expect that its just literally doing same shit as strcmp */
I32 strncmp(const char *s1, const char *s2, size_t n) {
    while (n > 0 && *s1 && (*s1 == *s2)) {
        s1++;
        s2++;
        n--;
    }
    if (n == 0) {
        return 0;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}


/*There is only 7 functions in here right now :
 * (memset, memcpy, strcpy, strcmp, memmove, strlen, strncmp).*/

// TODO :  strchr, strstr, strtol/atoi, qsort
