#include "../include/typint.h"
#include "../include/vga.h"
#include "../include/string.h"
#include "../include/memory/pmm.h"

void *memset(void *ptr, int value, size_t num)
{
    U8 *p = ptr;

    while (num--)
        *p++ = (U8)value;

    return ptr;
}
