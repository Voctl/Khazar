/*First of all, I am Divine Intellect
 *And I want to say that, i stole this ata driver from
 *CDos-x86 repository :sob: and yeah the owner
 *of this Operation System is my friend, so
 *i can stole this ata driver, dw guys , and thank u for this
 *driver my dear friend - Kanan Majidzada :mwah:
 *and yeah I am Divine Intellect*/
/*Btw its not just copypasta i did some modification on this driver
 *for KhazarOS and, yeah, its working cool asf :D */


#include "types.h"
#include "../../arch/x86_64/port_io.h"
#include "ata.h"

#define ATA_DATA        0x1F0
#define ATA_ERROR       0x1F1
#define ATA_SECCOUNT    0x1F2
#define ATA_LBA_LO      0x1F3
#define ATA_LBA_MID     0x1F4
#define ATA_LBA_HI      0x1F5
#define ATA_DRIVE_HEAD  0x1F6
#define ATA_STATUS      0x1F7
#define ATA_COMMAND     0x1F7

#define ATA_CMD_READ    0x20
#define ATA_CMD_WRITE   0x30

#define STATUS_BSY      0x80
#define STATUS_DRQ      0x08
#define STATUS_ERR      0x01

static int ata_poll_ready(U0)
{
    /* wait for BSY to clear */
    int timeout = 100000;
    while (timeout--) {
        U8 status = byte_i(ATA_STATUS);
        if (!(status & STATUS_BSY)) {
            if (status & STATUS_ERR) return -1;
            if (status & STATUS_DRQ) return 0;
        }
    }
    return -1; /* timeout */
}

I64 ata_init(U0){
    /* select master drive, LBA mode */
    byte_o(ATA_DRIVE_HEAD, 0xE0);
    io_wait();
    return 0;
}

I64 ata_read_sector(U32 lba, U8 *buffer)
{
    byte_o(ATA_DRIVE_HEAD, 0xE0 | ((lba >> 24) & 0x0F));
    byte_o(ATA_SECCOUNT, 1);
    byte_o(ATA_LBA_LO, (U8)(lba & 0xFF));
    byte_o(ATA_LBA_MID, (U8)((lba >> 8) & 0xFF));
    byte_o(ATA_LBA_HI, (U8)((lba >> 16) & 0xFF));
    byte_o(ATA_COMMAND, ATA_CMD_READ);

    if (ata_poll_ready() != 0) return -1;

    for (int i = 0; i < 256; i++) {
        U16 data;
        __asm__ volatile ("inw %1, %0" : "=a"(data) : "Nd"((U16)ATA_DATA));
        buffer[i * 2]     = (U8)(data & 0xFF);
        buffer[i * 2 + 1] = (U8)((data >> 8) & 0xFF);
    }

    return 0;
}

I64 ata_write_sector(U32 lba, const U8 *buffer)
{
    byte_o(ATA_DRIVE_HEAD, 0xE0 | ((lba >> 24) & 0x0F));
    byte_o(ATA_SECCOUNT, 1);
    byte_o(ATA_LBA_LO, (U8)(lba & 0xFF));
    byte_o(ATA_LBA_MID, (U8)((lba >> 8) & 0xFF));
    byte_o(ATA_LBA_HI, (U8)((lba >> 16) & 0xFF));
    byte_o(ATA_COMMAND, ATA_CMD_WRITE);

    if (ata_poll_ready() != 0) return -1;

    for (int i = 0; i < 256; i++) {
        U16 data = buffer[i * 2] | (buffer[i * 2 + 1] << 8);
        __asm__ volatile ("outw %0, %1" : : "a"(data), "Nd"((U16)ATA_DATA));
    }

    /* flush cache */
    byte_o(ATA_COMMAND, 0xE7);
    ata_poll_ready();

    return 0;
}
