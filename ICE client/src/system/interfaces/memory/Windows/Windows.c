internal function Byte* allocate_memory (N_32 size)
{
    //return VirtualAlloc(0, size, WINDOWS_COMMIT_MEMORY, WINDOWS_PAGE_EXECUTE_READWRITE);
    return malloc(size);
}


internal procedure free_memory(Byte *memory_address)
{
    //VirtualFree(memory_address, 0, WINDOWS_RELEASE_MEMORY);
    free(memory_address);
}


internal function Byte* reallocate_memory(Byte *data, N_32 size)
{
    return realloc(data, size);
}