private stdcall function N_32 recv   (Z_32 socket, Byte *data, N_32 length, N_32 flags);
//private stdcall function N_32 send   (Z_32 socket, Byte *data, N_32 length, N_32 flags);
private stdcall function N_32 sendto (Z_32 socket, Byte *data, N_32 length, N_32 flags, Address *address, N_32 address_length);


#include "Windows.c"
