typedef struct
{
    N_16  version;
    N_16  high_version;
    Byte  description[257];
    Byte  system_status[129];
    N_32  maximum_sockets;
    N_32  maximum_UDP_size;
    Byte *vendor_info;
}
Windows_Sockets;


internal Windows_Sockets windows_sockets = {0};


private stdcall function  N_32  WSAStartup      (N_16 version, Windows_Sockets *sockets);
private stdcall procedure       WSACleanup      ();
private stdcall function  N_32  WSAGetLastError ();


#include "Windows.c"
