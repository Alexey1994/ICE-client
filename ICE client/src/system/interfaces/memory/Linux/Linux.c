internal function Byte* allocate_memory (N_32 size)
{
    return malloc(size);
}


internal procedure free_memory (Byte *memory_address)
{
    free(memory_address);
}


internal function Byte* reallocate_memory (Byte *data, N_32 size)
{
    return realloc(data, size);
}