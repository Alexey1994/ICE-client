#ifndef MEMORY_H_INCLUDED
#define MEMORY_H_INCLUDED


internal function  Byte* allocate_memory   (N_32 size);
internal procedure       free_memory       (Byte *memory_address);
internal function  Byte* reallocate_memory (Byte *data, N_32 size);


#include "memory.c"

#endif // MEMORY_H_INCLUDED
