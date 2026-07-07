#ifndef TYPINT_H
#define TYPINT_H

/* KhazarOS types */

#define I8 signed char
#define U8 unsigned char
#define I16 signed short
#define U16 unsigned short
#define I32 signed int
#define U32 unsigned int
#define I64 signed long
#define U64 unsigned long

/* 64-bit system */
#define PTR_U64 unsigned long
#define PTR_I64 signed long

/* Memory and Size */
#define size_t unsigned long
#define ssize_t signed long
#define uintptr_t unsigned long

/* String */
#define STR8 U8 *
#define STR8_C const U8 *

/* fast */

#define I_FAST8 I8
#define U_FAST8 U8
#define I_FAST16 I32
#define U_FAST16 U32
#define I_FAST32 I32
#define U_FAST32 U32
#define I_FAST64 I64
#define U_FAST64 U64

/* Booleans */
#define BOOL I64
#define TRUE 1
#define FALSE 0

#endif
