#ifndef TYPINT_H
#define TYPINT_H

/* Baza tam ədəd tipləri */
typedef signed char        int8_t;
typedef unsigned char      uint8_t;
typedef short              int16_t;
typedef unsigned short     uint16_t;
typedef int                int32_t;
typedef unsigned int       uint32_t;
typedef long               int64_t;
typedef unsigned long      uint64_t;

/* Sürətli tam ədəd tipləri (Fast types) */
typedef char               intfast8_t;
typedef unsigned char      uintfast8_t;
typedef int                int_fast16_t;
typedef unsigned int       uint_fast16_t;
typedef int                int_fast32_t;
typedef unsigned int       uint_fast32_t;
typedef long               int_fast64_t;  
typedef unsigned long      uint_fast64_t; 

/* Sistem və yaddaş tipləri (x86_64 GCC standartı) */
typedef unsigned long      uintptr_t;
typedef unsigned long      size_t;
typedef long               ssize_t;

/* Köməkçi string tipləri */
typedef char* string;
typedef const char* string_const;

#endif
