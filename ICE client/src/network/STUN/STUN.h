#ifndef STUN_H_INCLUDED
#define STUN_H_INCLUDED


#include "../../input/input.h"
#include "../../output/output.h"
#include "../../data structures/buffer/buffer.h"
#include "../../system/devices/network/connection/UDP/UDP.h"
#include "../../mathematic/probability/random/random.h"
#include "../../mathematic/cryptography/cryptography.h"


//#define STUN_COOKIE 0x2112a442
internal const Byte STUN_COOKIE[] = {0x21, 0x12, 0xA4, 0x42};


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

/*
typedef struct
{
    Byte family;
    N_16 port;
    N_32 ip_v4;
    Byte ip_v6[16];
}
Address;*/


typedef struct
{
    UDP_Connection  *connection;
    Buffer           buffer;
    Output           buffer_output;
    Input            buffer_input;

    struct
    {
        struct
        {
            N_16 type;
            N_16 length;
            N_32 cookie;
            Byte transaction_id[12];
        }
        head;

        struct
        {
            N_16 type;
            N_16 length;
        }
        attribute;

        struct
        {
            Address MAPPED_ADDRESS;
            Buffer  NONCE;
            Buffer  REALM;
            N_32    ERROR_CODE;
        }
        attributes;
    }
    parser;
}
STUN_Client;


typedef struct
{
    UDP_Connection connection;

    Address local_address;
    Address remote_address;
}
STUN_Channel;


internal function Boolean initialize_STUN_channel(
    STUN_Channel *channel,

    Address *STUN_server_address,

    Byte *local_ID,
    procedure (*send_local_address)(Byte *local_ID, Address *local),

    Byte *remote_ID,
    procedure (*get_remote_address)(Byte *remote_ID, Address *remote)
);

internal procedure        deinitialize_STUN_channel     (STUN_Channel *channel);

internal function Boolean transmit_data_to_STUN_client  (STUN_Channel *channel, Byte *data, N_32 data_length);
internal function Boolean receive_data_from_STUN_client (STUN_Channel *channel, Byte *data, N_32 *data_length);


#include "STUN.c"

#endif //STUN_H_INCLUDED
