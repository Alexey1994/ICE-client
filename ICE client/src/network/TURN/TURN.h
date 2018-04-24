#ifndef TURN_H_INCLUDED
#define TURN_H_INCLUDED


#include "../STUN/STUN.h"


typedef enum
{
    ALLOCATE_TURN_MESSAGE          = 0x0003,
    REFRESH_TURN_MESSAGE           = 0x0004,
    SEND_TURN_MESSAGE              = 0x0016,
    DATA_TURN_MESSAGE              = 0x0017,
    CREATE_PERMISSION_TURN_MESSAGE = 0x0008,
    CHANNEL_BIND_TURN_MESSAGE      = 0x0009
}
TURN_Message_Type;


typedef enum
{
    CHANNEL_NUMBER_TURN_ATTRIBUTE      = 0x000C,
    LIFETIME_TURN_ATTRIBUTE            = 0x000D,
    XOR_PEER_ADDRESS_TURN_ATTRIBUTE    = 0x0012,
    DATA_TURN_ATTRIBUTE                = 0x0013,
    XOR_RELAYED_ADDRESS_TURN_ATTRIBUTE = 0x0016,
    EVEN_PORT_TURN_ATTRIBUTE           = 0x0018,
    REQUESTED_TRANSPORT_TURN_ATTRIBUTE = 0x0019,
    DONT_FRAGMENT_TURN_ATTRIBUTE       = 0x001A,
    RESERVATION_TOKEN_TURN_ATTRIBUTE   = 0x0022
}
TURN_Attribute_Type;


typedef struct
{
    STUN_Client STUN_client;

    struct
    {
        struct
        {
            Address XOR_RELAYED_ADDRESS;
            Address XOR_PEER_ADDRESS;

            struct
            {
                Address address;
                Buffer  data;
            }
            DATA;
        }
        attributes;
    }
    parser;
}
TURN_Client;


typedef struct
{
    UDP_Connection connection;
    Address        local;
    Address        remote;
}
TURN_Channel;


internal function Boolean initialize_TURN_Channel(
    TURN_Channel *channel,

    Address *TURN_server_address,

    Byte *username,
    Byte *password,

    Byte *local_ID,
    procedure (*send_local_address)(Byte *local_ID, Address *local),

    Byte *remote_ID,
    procedure (*get_remote_address)(Byte *remote_ID, Address *remote)
);


internal function Boolean transmit_data_to_TURN_client(TURN_Channel *channel, Buffer *data);
internal function Boolean receive_data_from_TURN_client(TURN_Channel *channel, Buffer *data);

internal procedure deinitialize_TURN_Channel(TURN_Channel *channel);


#include "TURN.c"

#endif //TURN_H_INCLUDED
