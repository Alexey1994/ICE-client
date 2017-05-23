// RFC 5766: TURN RFC

#ifndef TURN_H_INCLUDED
#define TURN_H_INCLUDED


#include "../STUN/STUN.h"


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
    ALLOCATE_TURN_MESSAGE          = 0x0300,
    REFRESH_TURN_MESSAGE           = 0x0400,
    SEND_TURN_MESSAGE              = 0x0600,
    DATA_TURN_MESSAGE              = 0x0700,
    CREATE_PERMISSION_TURN_MESSAGE = 0x0800,
    CHANNEL_BIND_TURN_MESSAGE      = 0x0900
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
void TURN(char *host, short port);

#include "TURN.c"

#endif // TURN_H_INCLUDED
