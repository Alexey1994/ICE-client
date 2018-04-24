#include "../../../../interfaces/thread/thread.h"


#define NON_BLOCKING_MODE         0x8004667e

#define RESOURCE_BLOCKED          10035
#define SOCKET_ALREADY            10037
#define SOCKET_CONNECTED          10056


private stdcall function  N_16             htons         (N_16 port);
private stdcall function  Z_32             socket        (N_32 family, N_32 type, N_32 protocol);
private stdcall function  N_32             closesocket   (Z_32 socket);
private stdcall function  N_32             connect       (Z_32 conection, Address *address, N_32 size_of_address);
private stdcall function  N_32             bind          (Z_32 conection, Address *address, N_32 size_of_address);
private stdcall function  N_32             ioctlsocket   (Z_32 socket, N_32 command, Byte *arguments);
private stdcall function  N_32             recvfrom      (Z_32 conection, Byte *data, N_32 length, N_32 flags, Address *address, N_32 size_of_address);


#include "Windows.c"
