#ifndef EXTENDS_H_INCLUDED
#define EXTENDS_H_INCLUDED


#include <stdint.h>
#include <stdbool.h>


#define loop       for(;;)
#define increment  ++
#define private    static
#define internal   static
#define procedure  void
#define function


typedef int8_t   Z_8;
typedef int16_t  Z_16;
typedef int32_t  Z_32;
typedef int64_t  Z_64;

typedef uint8_t  N_8;
typedef uint16_t N_16;
typedef uint32_t N_32;
typedef uint64_t N_64;

typedef uint8_t  Byte;
typedef bool     Boolean;


#include "../interfaces/memory/memory.h"


#define new(type)                             allocate_memory(sizeof(type))
#define new_array(type, length)               allocate_memory(sizeof(type)*length)

#define reallocate(data, type)                data=reallocate_memory(data, sizeof(type))
#define reallocate_array(data, type, length)  data=reallocate_memory(data, sizeof(type)*length)

internal procedure revert_bytes_order (Byte *array, N_32 length);
internal procedure swap               (Byte *element1, Byte *element2, N_32 size_of_element);

#ifdef BIG_ENDIAN
    #define convert_little_to_big_endian(array, length)
    #define convert_big_to_little_endian                revert_bytes_order
#else
    #define convert_little_to_big_endian                revert_bytes_order
    #define convert_big_to_little_endian(array, length)
#endif


#include "extends.c"

#endif // EXTENDS_H_INCLUDED
