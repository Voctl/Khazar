#include "commands.h"
#include "types.h"
#include "vga.h"
#include "../../fs/atadrv/ata.h"

/* diskread <hex_lba>
 * Reads one 512-byte sector from the ATA master drive and hex-dumps it.
 * Example: diskread 0   -> dumps the boot sector (LBA 0)
 */
U0 cmd_diskread(char *args)
{
    U32 lba;
    U8 sector[512];
    I32 row, col;

    if (!args || !*args) {
        putstr_color((STR8_C) " [ ERROR ] ", COLOR_RED);
        putstr((STR8_C) "usage: diskread <hex_lba>\n");
        return;
    }

    lba = strtohex(args);

    if (ata_read_sector(lba, sector) != 0) {
        putstr_color((STR8_C) " [ ERROR ] ", COLOR_RED);
        putstr((STR8_C) "ATA read failed (timeout/err)\n");
        return;
    }

    putstr_color((STR8_C) "[ OK ] ", COLOR_LIGHT_GREEN);
    putstr((STR8_C) "sector read, dumping first 128 bytes:\n");

    for (row = 0; row < 8; row++) {
        for (col = 0; col < 16; col++) {
            puthex(sector[row * 16 + col]);
            putstr((STR8_C) " ");
        }
        putstr((STR8_C) "\n");
    }
}
