#include "../include/typint.h"
#include "../include/vga.h"
#include "../include/string.h"
#include "../include/memory/pmm.h"

void *memset(void *ptr, int value, size_t num)
{
    unsigned char *p = ptr;

    while (num--)
        *p++ = (unsigned char)value;

    return ptr;
}
