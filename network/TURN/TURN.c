#include "TURN.h"
#include "../../data structures/string/string.h"
#include "../../logger/logger.h"

#include "debug.h"

#include "attributes/DATA.c"


static void(*read_attribute_TURN_handlers[65536])(TURN_Attributes *attributes, Byte *attribute, int attribute_length);


void initialize_TURN()
{
    int i;

    for(i=0; i<65536; ++i)
        read_attribute_TURN_handlers[i] = 0;

#if ENABLE_TURN_DEBUG
    initialize_TURN_debug();
#endif
}


void read_TURN_attribute(TURN_Attribute *attribute, TURN_Attributes *attributes)
{
    void(*TURN_attribute_handler) (TURN_Attributes *attributes, Byte *attribute, int attribute_length);
    TURN_attribute_handler = read_attribute_TURN_handlers[ attribute->type ];

    if(!TURN_attribute_handler)
    {
        void(*STUN_attribute_handler) (STUN_Attributes *attributes, Byte *attribute, int attribute_length);
        STUN_attribute_handler = read_attribute_handlers[ attribute->type ];

        if(!STUN_attribute_handler)
            return;

        STUN_attribute_handler(attributes, (Byte*)attribute + 4, attribute->length);

        return;
    }

    TURN_attribute_handler(attributes, (Byte*)attribute + 4, attribute->length);
}


void read_TURN_attributes(TURN_Attributes *attributes, String *message)
{
    int             length     = 20;
    STUN_Attribute *attribute  = message->begin + 20;

    while(length < message->length)
    {
        convert_big_to_little_endian(&attribute->type, 2);
        convert_big_to_little_endian(&attribute->length, 2);
        read_TURN_attribute(attribute, attributes);
        length += 4 + attribute->length;
        attribute = (Byte*)attribute + 4 + attribute->length;
    }
}


static TURN_Attributes* TURN_request(NetworkConnection connection)
{
    String *response_message;

    TURN_Attributes   *attributes        = new(TURN_Attributes);
    String            *head              = create_string(20);
    String            *source_attributes = create_string(10);

    //attributes->MAPPRED_ADDRESS.host = 0;
    //attributes->CHANGED_ADDRESS.host = 0;

    add_request_head(head, source_attributes->length);
    concatenate_strings(head, source_attributes);
    destroy_string(source_attributes);

    request(connection, head);

#if ENABLE_TURN_DEBUG
    print_TURN_request(head);
#endif

    response_message = response(connection);

    if(!response_message)
        goto error;

    read_TURN_attributes(attributes, response_message);

#if ENABLE_TURN_DEBUG
    print_TURN_response(response_message);
#endif

    destroy_network_connection(connection);

    return attributes;

error:
    return 0;
}


void add_ALLOCATION_request_head(String *message, int content_length)
{
    STUN_Header *header      =  message->begin;

    header->message_type   = ALLOCATE_TURN_MESSAGE;
    header->message_length = content_length;
    header->magic_cookie   = STUN_COOKIE;
    generate_transaction_ID(header->transaction_ID);

    message->length = 20;
}


static TURN_Attributes* TURN_ALLOCATION_request(NetworkConnection connection)
{
    String *response_message;

    TURN_Attributes   *attributes        = new(TURN_Attributes);
    String            *head              = create_string(20);
    String            *source_attributes = create_string(10);

    //attributes->MAPPRED_ADDRESS.host = 0;
    //attributes->CHANGED_ADDRESS.host = 0;

    add_ALLOCATION_request_head(head, source_attributes->length);
    concatenate_strings(head, source_attributes);
    destroy_string(source_attributes);

    request(connection, head);

#if ENABLE_TURN_DEBUG
    print_TURN_request(head);
#endif

    response_message = response(connection);

    if(!response_message)
        goto error;

    read_TURN_attributes(attributes, response_message);

#if ENABLE_TURN_DEBUG
    print_TURN_response(response_message);
#endif

    destroy_network_connection(connection);

    return attributes;

error:
    return 0;
}


TURN_Attributes* TURN_TCP_request(char *host, int port)
{
    NetworkConnection  connection  =  create_TCP_connection(host, port);

    if(!connection)
        goto error;

    return TURN_request(connection);

error:
    return 0;
}


TURN_Attributes* TURN_UDP_request(char *host, int port)
{
    NetworkConnection  connection  =  create_UDP_connection(host, port);

    if(!connection)
        goto error;

    //return TURN_request(connection);
    return TURN_ALLOCATION_request(connection);

error:
    return 0;
}
