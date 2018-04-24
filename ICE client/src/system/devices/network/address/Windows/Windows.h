typedef struct
{
    N_32                  flags;
    N_32                  family;
    N_32                  socket_type;
    N_32                  protocol;
    N_32                  address_length;
    Byte                 *name;
    Address              *address;
    struct Address_Info  *next;
}
Address_Info;


private stdcall function  Address_Info* getaddrinfo   (Byte *host, Byte *port, Address_Info *hints, Address_Info **result);
private stdcall procedure               freeaddrinfo  (Address_Info *address_info);


#include "Windows.c"
