#include "fat16.h"
#include "types.h"
#include "vga.h"
#include "string.h"
#include "../atadrv/ata.h"

/* Our ATA driver only speaks 512-byte sectors right now, so we require
 * the filesystem to use 512-byte sectors too (true for basically every
 * mkfs.fat -F 16 image at these sizes). */
#define SECTOR_SIZE     512
#define DIR_ENTRY_SIZE  32
#define ENTRIES_PER_SECTOR (SECTOR_SIZE / DIR_ENTRY_SIZE)

#define ATTR_LONG_NAME  0x0F
#define ATTR_VOLUME_ID  0x08
#define ATTR_DIRECTORY  0x10

/* BIOS Parameter Block, as laid out in the boot sector. */
typedef struct __attribute__((packed)) {
    U8  jmp[3];
    U8  oem[8];
    U16 bytes_per_sector;
    U8  sectors_per_cluster;
    U16 reserved_sectors;
    U8  num_fats;
    U16 root_entry_count;
    U16 total_sectors_16;
    U8  media_type;
    U16 fat_size_16;
    U16 sectors_per_track;
    U16 num_heads;
    U32 hidden_sectors;
    U32 total_sectors_32;
} fat16_bpb_t;

/* One 32-byte root directory entry. */
typedef struct __attribute__((packed)) {
    U8  name[8];
    U8  ext[3];
    U8  attr;
    U8  reserved[10];
    U16 time;
    U16 date;
    U16 first_cluster;
    U32 file_size;
} fat16_dirent_t;

static U32 bytes_per_sector;
static U32 sectors_per_cluster;
static U32 reserved_sectors;
static U32 num_fats;
static U32 root_entry_count;
static U32 fat_size_sectors;
static U32 fat_start_sector;
static U32 root_dir_sector;
static U32 root_dir_sectors;
static U32 first_data_sector;
static U8  ready = 0;

/* forward decls so definition order below doesn't matter */
static U0 to_fat_name(const char *input, U8 *name8, U8 *ext3);
static I32 fat16_memcmp(const U8 *a, const U8 *b, U32 n);
static I64 read_root_dir_sector(U32 sector_index, fat16_dirent_t *out16);
static I64 find_entry(const char *name, fat16_dirent_t *found);
static U32 next_cluster(U32 cluster);

I64 fat16_init(U0)
{
    U8 sector[SECTOR_SIZE];
    fat16_bpb_t *bpb;

    ready = 0;

    if (ata_read_sector(0, sector) != 0) {
        return -1;
    }

    bpb = (fat16_bpb_t *)sector;

    bytes_per_sector    = bpb->bytes_per_sector;
    sectors_per_cluster = bpb->sectors_per_cluster;
    reserved_sectors    = bpb->reserved_sectors;
    num_fats            = bpb->num_fats;
    root_entry_count    = bpb->root_entry_count;
    fat_size_sectors    = bpb->fat_size_16;

    if (bytes_per_sector != SECTOR_SIZE) {
        return -1; /* unsupported: needs a 512-byte-sector filesystem */
    }
    if (sectors_per_cluster == 0 || num_fats == 0) {
        return -1; /* not a valid-looking FAT16 BPB */
    }

    fat_start_sector  = reserved_sectors;
    root_dir_sector   = reserved_sectors + (num_fats * fat_size_sectors);
    root_dir_sectors  = ((root_entry_count * DIR_ENTRY_SIZE) + (bytes_per_sector - 1)) / bytes_per_sector;
    first_data_sector = root_dir_sector + root_dir_sectors;

    ready = 1;
    return 0;
}

/* Converts "readme.txt" -> name8="README  " ext3="TXT", both
 * space-padded and upper-cased, matching how FAT stores 8.3 names. */
static U0 to_fat_name(const char *input, U8 *name8, U8 *ext3)
{
    I32 i;
    const char *dot;
    U32 name_len;

    for (i = 0; i < 8; i++) name8[i] = ' ';
    for (i = 0; i < 3; i++) ext3[i]  = ' ';

    dot = input;
    while (*dot && *dot != '.') dot++;
    name_len = (U32)(dot - input);
    if (name_len > 8) name_len = 8;

    for (i = 0; i < (I32)name_len; i++) {
        char c = input[i];
        if (c >= 'a' && c <= 'z') c = (char)(c - 'a' + 'A');
        name8[i] = (U8)c;
    }

    if (*dot == '.') {
        dot++;
        for (i = 0; i < 3 && dot[i]; i++) {
            char c = dot[i];
            if (c >= 'a' && c <= 'z') c = (char)(c - 'a' + 'A');
            ext3[i] = (U8)c;
        }
    }
}

/* small local memcmp so we don't depend on libk having one */
static I32 fat16_memcmp(const U8 *a, const U8 *b, U32 n)
{
    U32 i;
    for (i = 0; i < n; i++) {
        if (a[i] != b[i]) return (I32)a[i] - (I32)b[i];
    }
    return 0;
}

static I64 read_root_dir_sector(U32 sector_index, fat16_dirent_t *out16)
{
    return ata_read_sector(root_dir_sector + sector_index, (U8 *)out16);
}

U0 fat16_list(U0)
{
    fat16_dirent_t entries[ENTRIES_PER_SECTOR];
    U32 s, i;

    if (!ready) {
        putstr_color((STR8_C) " [ ERROR ] ", COLOR_RED);
        putstr((STR8_C) "fat16 not initialized (no/bad filesystem on disk)\n");
        return;
    }

    for (s = 0; s < root_dir_sectors; s++) {
        if (read_root_dir_sector(s, entries) != 0) {
            putstr_color((STR8_C) " [ ERROR ] ", COLOR_RED);
            putstr((STR8_C) "disk read failed while listing\n");
            return;
        }

        for (i = 0; i < ENTRIES_PER_SECTOR; i++) {
            fat16_dirent_t *e = &entries[i];
            U8 first = e->name[0];
            I32 c;

            if (first == 0x00) return;       /* no more entries at all */
            if (first == 0xE5) continue;     /* deleted entry */
            if (e->attr == ATTR_LONG_NAME) continue;
            if (e->attr & ATTR_VOLUME_ID) continue;
            if (e->attr & ATTR_DIRECTORY) continue; /* subdirs: not yet supported */

            for (c = 0; c < 8 && e->name[c] != ' '; c++) {
                U8 ch[2];
                ch[0] = e->name[c];
                ch[1] = 0;
                putstr((STR8_C)ch);
            }
            if (e->ext[0] != ' ') {
                putstr((STR8_C) ".");
                for (c = 0; c < 3 && e->ext[c] != ' '; c++) {
                    U8 ch[2];
                    ch[0] = e->ext[c];
                    ch[1] = 0;
                    putstr((STR8_C)ch);
                }
            }
            putstr((STR8_C) "   ");
            putdec(e->file_size);
            putstr((STR8_C) " bytes\n");
        }
    }
}

static I64 find_entry(const char *name, fat16_dirent_t *found)
{
    U8 want_name[8], want_ext[3];
    fat16_dirent_t entries[ENTRIES_PER_SECTOR];
    U32 s, i;

    to_fat_name(name, want_name, want_ext);

    for (s = 0; s < root_dir_sectors; s++) {
        if (read_root_dir_sector(s, entries) != 0) return -1;

        for (i = 0; i < ENTRIES_PER_SECTOR; i++) {
            fat16_dirent_t *e = &entries[i];
            U8 first = e->name[0];

            if (first == 0x00) return -1; /* end of directory */
            if (first == 0xE5) continue;
            if (e->attr == ATTR_LONG_NAME) continue;
            if (e->attr & ATTR_VOLUME_ID) continue;
            if (e->attr & ATTR_DIRECTORY) continue;

            if (fat16_memcmp(e->name, want_name, 8) == 0 &&
                fat16_memcmp(e->ext, want_ext, 3) == 0) {
                *found = *e;
                return 0;
            }
        }
    }
    return -1;
}

static U32 next_cluster(U32 cluster)
{
    U8 sector[SECTOR_SIZE];
    U32 fat_byte_offset  = cluster * 2;
    U32 fat_sector       = fat_start_sector + (fat_byte_offset / bytes_per_sector);
    U32 offset_in_sector = fat_byte_offset % bytes_per_sector;

    if (ata_read_sector(fat_sector, sector) != 0) {
        return 0xFFFF; /* treat read error as end-of-chain */
    }
    return (U32)sector[offset_in_sector] | ((U32)sector[offset_in_sector + 1] << 8);
}

I64 fat16_read(const char *name, U8 *buffer, U32 buffer_size)
{
    fat16_dirent_t entry;
    U32 cluster, bytes_left, bytes_written;

    if (!ready) return -1;
    if (find_entry(name, &entry) != 0) return -1;

    cluster       = entry.first_cluster;
    bytes_left    = entry.file_size;
    bytes_written = 0;

    if (bytes_left > buffer_size) return -1; /* caller's buffer too small */

    while (cluster >= 0x0002 && cluster < 0xFFF8 && bytes_left > 0) {
        U32 lba = first_data_sector + (cluster - 2) * sectors_per_cluster;
        U32 sec;

        for (sec = 0; sec < sectors_per_cluster && bytes_left > 0; sec++) {
            U8 sector[SECTOR_SIZE];
            U32 chunk = (bytes_left < SECTOR_SIZE) ? bytes_left : SECTOR_SIZE;

            if (ata_read_sector(lba + sec, sector) != 0) return -1;

            memcpy((void *)(buffer + bytes_written), sector, chunk);
            bytes_written += chunk;
            bytes_left    -= chunk;
        }

        cluster = next_cluster(cluster);
    }

    return (I64)bytes_written;
}
