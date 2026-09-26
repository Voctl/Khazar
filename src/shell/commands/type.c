#include "commands.h"
#include "types.h"
#include "vga.h"
#include "../../fs/fat16/fat16.h"

#define TYPE_BUF_SIZE 8192

/* static, not a stack local: the kernel stack is only 4096 bytes total
 * (KERNEL_STACK_SIZE in loader.s), so an 8KB local array here would
 * silently overflow it. */
static U8 type_buf[TYPE_BUF_SIZE];

U0 cmd_type(char *args)
{
    I64 n;

    if (!args || !*args) {
        putstr_color((STR8_C) " [ ERROR ] ", COLOR_RED);
        putstr((STR8_C) "usage: type <filename>\n");
        return;
    }

    /* leave room for our own null terminator */
    n = fat16_read(args, type_buf, TYPE_BUF_SIZE - 1);

    if (n < 0) {
        putstr_color((STR8_C) " [ ERROR ] ", COLOR_RED);
        putstr((STR8_C) "file not found or too large: ");
        putstr((STR8_C)args);
        putstr((STR8_C) "\n");
        return;
    }

    type_buf[n] = '\0';
    putstr((STR8_C)type_buf);
    putstr((STR8_C) "\n");
}
