// RFC 5766: TURN RFC

#ifndef TURN_H_INCLUDED
#define TURN_H_INCLUDED


#include "../STUN/STUN.h"


#define MAX_TURN_RESPONSE_LENGTH 512


typedef enum
{
    TCP_CONNECTION,
    UDP_CONNECTION
}
Connection_Type;


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
    unsigned short type;
    unsigned short length;
}
TURN_Attribute;


typedef enum
{
    ALLOCATE_TURN_MESSAGE          = 0x0003,
    REFRESH_TURN_MESSAGE           = 0x0004,
    SEND_TURN_MESSAGE              = 0x0006,
    DATA_TURN_MESSAGE              = 0x0007,
    CREATE_PERMISSION_TURN_MESSAGE = 0x0008,
    CHANNEL_BIND_TURN_MESSAGE      = 0x0009
}
TURN_Message_Type;


typedef struct
{
    struct
    {
        Byte           *host;
        unsigned short  port;
    }DATA;

    STUN_Attributes *STUN_attributes;
}
TURN_Attributes;


void initialize_TURN();

void allocate_TURN(char *host, short port);
void bind_TURN_channel(char *host, short port);

#include "TURN.c"

#endif // TURN_H_INCLUDED
