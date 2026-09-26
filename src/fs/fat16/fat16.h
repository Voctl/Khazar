#ifndef FAT16_H
#define FAT16_H

#include "types.h"

I64 fat16_init(U0);
U0 fat16_list(U0);
I64 fat16_read(const char *name, U8 *buffer, U32 buffer_size);

#endif /* FAT16_H */
