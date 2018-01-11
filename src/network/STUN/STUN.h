// RFC 5389: STUN RFC
// RFC 5769: test vectors for STUN

#ifndef STUN_H_INCLUDED
#define STUN_H_INCLUDED


#include "../../extends/extends.h"
#include "../../data structures/string/string.h"
#include "../connection/UDP/UDP.h"


#define MAX_STUN_RESPONSE_LENGTH 512


#define DEFAULT_STUN_PORT  3478
#define	STUN_COOKIE        0x42a41221


typedef enum
{
    MAPPED_ADDRESS_STUN_ATTRIBUTE     = 0x0001,
    RESPONSE_ADDRESS_STUN_ATTRIBUTE   = 0x0002,
    CHANGE_REQUEST_STUN_ATTRIBUTE     = 0x0003,
    SOURCE_ADDRESS_STUN_ATTRIBUTE     = 0x0004,
    CHANGED_ADDRESS_STUN_ATTRIBUTE    = 0x0005,
    USERNAME_STUN_ATTRIBUTE           = 0x0006,
    PASSWORD_STUN_ATTRIBUTE           = 0x0007,
    MESSAGE_INTEGRITY_STUN_ATTRIBUTE  = 0x0008,
    ERROR_CODE_STUN_ATTRIBUTE         = 0x0009,
    UNKNOWN_ATTRIBUTES_STUN_ATTRIBUTE = 0x000a,
    REFLECTED_FROM_STUN_ATTRIBUTE     = 0x000b,
    REALM_STUN_ATTRIBUTE              = 0x0014,
    NONCE_STUN_ATTRIBUTE              = 0x0015,
    XOR_MAPPED_ADDRESS_STUN_ATTRIBUTE = 0x0020,

    SOFTWARE_STUN_ATTRIBUTE           = 0x8022,
    ALTERNATE_SERVER_STUN_ATTRIBUTE   = 0x8023,
    FINGERPRINT_STUN_ATTRIBUTE        = 0x8028
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
    BINDING_REQUEST              = 0x0001,
    BINDING_RESPONSE             = 0x0101,
    BINDING_ERROR_RESPONSE       = 0x0111,
    SHARED_SECRET_REQUEST        = 0x0002,
    SHARED_SECRET_RESPONSE       = 0x0102,
    SHARED_SECRET_ERROR_RESPONSE = 0x0112
}
STUN_Message_Type;


typedef struct
{
    unsigned short  message_type;
    unsigned short  content_length;
    long int        magic_cookie;
    Byte            transaction_ID[12];
}
STUN_Head;


typedef struct
{
    struct
    {
        Byte           *host;
        unsigned short  port;
    }MAPPED_ADDRESS;

    struct
    {
        Byte           *host;
        unsigned short  port;
    }CHANGED_ADDRESS;

    struct
    {
        N_8   length;
        Byte *data;
    }NONCE;

    Character *REALM;
    N_16       ERROR_CODE;
}
STUN_Attributes;


void    initialize_STUN();
String* create_STUN_head(unsigned short type);
Boolean get_STUN_mapped_address(char *host, unsigned short port, char *mapped_host, unsigned short *mapped_port);
Boolean authenticate_on_STUN_server(char *host, unsigned short port);


#include "STUN.c"

#endif // STUN_H_INCLUDED
