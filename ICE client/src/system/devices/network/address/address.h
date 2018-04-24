#ifndef NETWORK_ADDRESS_H_INCLUDED
#define NETWORK_ADDRESS_H_INCLUDED


#include "../../../../output/output.h"

//#include "../network.h"

#define ANY_INTERNET_PROTOCOL     0
#define IPv4                      2
#define IPv6                      23

#define SOCKET_STREAM             1
#define SOCKET_DATAGRAMS          2

#define TCP_PROTOCOL              6
#define UDP_PROTOCOL              17


typedef struct
{
    N_8  length;
    N_8  family;
    N_16 port;

    union
    {
        N_32 ip_v4;
        N_32 ip_v6_flow_info;
    };

    Byte ip_v6[16];

    union
    {
        N_32 ip_v6_scope_id;

        union
        {
            struct
            {
                N_32 zone: 28;
                N_32 level: 4;
            };

            N_32 value;
        };
    };
}
Address;


internal function Boolean initialize_address (Address *address, Byte *host, Byte *port, N_32 protocol);
internal function Boolean write_address      (Output *output, Address *address);


#include "address.c"

#endif // NETWORK_ADDRESS_H_INCLUDED
