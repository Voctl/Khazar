#pragma once
#include "types.h"

void *memset(void *ptr, int value, size_t num);
void *memcpy(void *dest, const void *src, size_t num);
char *strcpy(char *dest, const char *src);
I32 strcmp(const char *s1, const char *s2);
// tdy
void *memmove(void *dest, const void *src, size_t num);
size_t strlen(const char *s);
I32 strncmp(const char *s1, const char *s2, size_t n);
