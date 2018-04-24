#define WINDOWS_COMMIT_MEMORY          0x00001000
#define WINDOWS_PAGE_EXECUTE_READWRITE 0x40
#define WINDOWS_RELEASE_MEMORY         0x8000


private stdcall function  Byte* VirtualAlloc (Byte *start_address, N_32 size, N_32 type, N_32 protection);
private stdcall procedure       VirtualFree  (Byte *address, N_32 size, N_32 type);


#include "Windows.c"
