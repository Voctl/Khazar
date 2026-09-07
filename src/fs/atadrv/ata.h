/*First of all, I am Divine Intellect
 *And I want to say that, i stole this ata driver from
 *CDos-x86 repository :sob: and yeah the owner
 *of this Operation System is my friend, so
 *i can stole this ata driver, dw guys , and thank u for this
 *driver my dear friend - Kanan Majidzada :mwah:
 *and yeah I am Divine Intellect*/
/*Btw its not just copypasta i did some modification on this driver
 *for KhazarOS and, yeah, its working cool asf :D */

#ifndef ATA_H
#define ATA_H

/* Returns 0 on success, non-zero on error */
I64 ata_init(U0);
I64 ata_read_sector(U32 lba, U8 *buffer);
I64 ata_write_sector(U32 lba, const U8 *buffer);


#endif /* ATA_H */
