// RFC 5389: STUN RFC
// RFC 5769: test vectors for STUN

#ifndef STUN_H_INCLUDED
#define STUN_H_INCLUDED


#include <libs/extends/extends.h>
#include <libs/network/connection/network connection.h>


#define DEFAULT_STUN_PORT  3478
#define	STUN_COOKIE        0x2112a442


typedef enum
{
    MAPPED_ADDRESS     = 0x0001,
    RESPONSE_ADDRESS   = 0x0002,
    CHANGE_REQUEST     = 0x0003,
    SOURCE_ADDRESS     = 0x0004,
    CHANGED_ADDRESS    = 0x0005,
    USERNAME           = 0x0006,
    PASSWORD           = 0x0007,
    MESSAGE_INTEGRITY  = 0x0008,
    ERROR_CODE         = 0x0009,
    UNKNOWN_ATTRIBUTES = 0x000a,
    REFLECTED_FROM     = 0x000b,
    REALM              = 0x0014,
    NONCE              = 0x0015,
    XOR_MAPPED_ADDRESS = 0x0020,

    SOFTWARE           = 0x8022,
    ALTERNATE_SERVER   = 0x8023,
    FINGERPRINT        = 0x8028
}
STUN_Attribute_Type;


typedef struct
{
    unsigned short type;
    unsigned short length;
}
STUN_Attribute;


typedef enum
{
    BINDING_REQUEST              = 0x0100,
    BINDING_RESPONSE             = 0x0101,
    BINDING_ERROR_RESPONSE       = 0x1101,
    SHARED_SECRET_REQUEST        = 0x0200,
    SHARED_SECRET_RESPONSE       = 0x0201,
    SHARED_SECRET_ERROR_RESPONSE = 0x1201
}
STUN_Message_Type;


typedef struct
{
    unsigned short  message_type;
    unsigned short  message_length;
    long int        magic_cookie;
    Byte            transaction_ID[12];
}
STUN_Header;


typedef struct
{
    struct
    {
        Byte           *host;
        unsigned short  port;
    }MAPPRED_ADDRESS;
}
STUN_Attributes;


void              initialize_STUN();
STUN_Attributes*  STUN_request(char *host, int port);
Byte              get_STUN_MAPPED_ADDRESS(char *STUN_host, unsigned short STUN_port, char *mapped_host, unsigned short *mapped_port);
NetworkConnection connect_to_STUN_MAPPED_ADDRESS(char *STUN_host, int STUN_port);
void              get_NAT_type_using_STUN_server(char *host, unsigned short port);


#endif // STUN_H_INCLUDED
