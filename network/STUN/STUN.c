#include "../../stream/stream.h"
#include "../../data structures/string/string.h"
#include "../../logger/logger.h"

#include "debug.h"

#include "attributes/MAPPED_ADDRESS.c"
#include "attributes/RESPONSE_ADDRESS.c"
#include "attributes/CHANGE_REQUEST.c"
#include "attributes/SOURCE_ADDRESS.c"
#include "attributes/CHANGED_ADDRESS.c"
#include "attributes/USERNAME.c"
#include "attributes/PASSWORD.c"
#include "attributes/MESSAGE_INTEGRITY.c"
#include "attributes/ERROR_CODE.c"
#include "attributes/UNKNOWN_ATTRIBUTES.c"
#include "attributes/REFLECTED_FROM.c"
#include "attributes/REALM.c"
#include "attributes/NONCE.c"
#include "attributes/XOR_MAPPED_ADDRESS.c"
#include "attributes/SOFTWARE.c"
#include "attributes/ALTERNATE_SERVER.c"
#include "attributes/FINGERPRINT.c"

#include "head.c"
#include "attribute reader.c"
#include "message.c"


void initialize_STUN()
{
    read_attribute_handlers[MAPPED_ADDRESS]     = read_MAPPED_ADDRESS_attribute;
    read_attribute_handlers[RESPONSE_ADDRESS]   = read_RESPONSE_ADDRESS_attribute;
    read_attribute_handlers[CHANGE_REQUEST]     = read_CHANGE_REQUEST_attribute;
    read_attribute_handlers[SOURCE_ADDRESS]     = read_SOURCE_ADDRESS_attribute;
    read_attribute_handlers[CHANGED_ADDRESS]    = read_CHANGED_ADDRESS_attribute;
    read_attribute_handlers[USERNAME]           = read_USERNAME_attribute;
    read_attribute_handlers[PASSWORD]           = read_PASSWORD_attribute;
    read_attribute_handlers[MESSAGE_INTEGRITY]  = read_MESSAGE_INTEGRITY_attribute;
    read_attribute_handlers[ERROR_CODE]         = read_ERROR_CODE_attribute;
    read_attribute_handlers[UNKNOWN_ATTRIBUTES] = read_UNKNOWN_ATTRIBUTES_attribute;
    read_attribute_handlers[REFLECTED_FROM]     = read_REFLECTED_FROM_attribute;
    read_attribute_handlers[REALM]              = read_REALM_attribute;
    read_attribute_handlers[NONCE]              = read_NONCE_attribute;
    read_attribute_handlers[XOR_MAPPED_ADDRESS] = read_XOR_MAPPED_ADDRESS_attribute;
    read_attribute_handlers[SOFTWARE]           = read_SOFTWARE_attribute;
    read_attribute_handlers[ALTERNATE_SERVER]   = read_ALTERNATE_SERVER_attribute;
    read_attribute_handlers[FINGERPRINT]        = read_FINGERPRINT_attribute;

#if ENABLE_STUN_DEBUG
    initialize_STUN_debug();
#endif
}


void STUN_request(NetworkConnection  connection, String *message)
{
    write_in_network_connection(connection, message->begin, message->length);

#if ENABLE_STUN_DEBUG
    print_STUN_request(message);
#endif
}


static void STUN_response_handler(Byte *data, Byte *end_response)
{
    if(!data)
    {
        *end_response = TIMEOUT_ERROR;
        return;
    }

    *end_response = 1;
}


String* STUN_response(NetworkConnection connection)
{
    STUN_Head *head;
    Byte       end_response;

    String *message = create_string(200);

    async_read_from_network_connection(connection, 500, message->begin, 200, STUN_response_handler, &end_response);

    while(!end_response);// waiting

    if(end_response == TIMEOUT_ERROR)
        goto error;

    head = message->begin;
    convert_big_to_little_endian(&head->content_length, 2);
    message->length = 20 + head->content_length;

#if ENABLE_STUN_DEBUG
    print_STUN_response(message);
#endif

    return message;

error:
    destroy_string(message);
    return 0;
}


STUN_Attributes* create_STUN_attributes()
{
    STUN_Attributes *attributes  = new(STUN_Attributes);

    attributes->MAPPRED_ADDRESS.host = 0;
    attributes->CHANGED_ADDRESS.host = 0;

    return attributes;
}


void destroy_STUN_attributes(STUN_Attributes *attributes)
{
    free(attributes->MAPPRED_ADDRESS.host);
    free(attributes);
}


Boolean get_STUN_mapped_address(char *host, unsigned short port, char *mapped_host, unsigned short *mapped_port)
{
    NetworkConnection  connection  =  create_UDP_connection(host, port);

    if(!connection)
        goto error;

    STUN_Attributes *attributes;
    String          *response_message;

    String *request_message = create_STUN_head(BINDING_REQUEST);
    end_STUN_message(request_message);

    STUN_request(connection, request_message);
    destroy_string(request_message);

    response_message = STUN_response(connection);
    destroy_network_connection(connection);

    if(!response_message)
        goto error;

    attributes = create_STUN_attributes_from_message(response_message);

#if ENABLE_STUN_DEBUG
    print_STUN_response(response_message);
#endif

    strcpy(mapped_host, attributes->MAPPRED_ADDRESS.host);
    *mapped_port = attributes->MAPPRED_ADDRESS.port;

    destroy_network_connection(connection);
    destroy_STUN_attributes(attributes);

    return 1;

error:
    return 0;
}


Boolean authenticate_on_STUN_server(char *host, unsigned short port)
{
    NetworkConnection  connection  =  create_UDP_connection(host, port);

    if(!connection)
        goto error;

    STUN_Attributes *attributes;
    String          *response_message;

    String *request_message = create_STUN_head(BINDING_REQUEST);
/*
    add_USERNAME_attribute_to_STUN_message(request_message, "asdf");
    add_NONCE_attribute(request_message, "2e131a5fb210812c");
    add_REALM_attribute(request_message);

    add_MESSAGE_INTEGRITY_attribute(request_message);*/

    add_USERNAME_attribute_to_STUN_message(request_message, "asdf");
    //add_MESSAGE_INTEGRITY_attribute(request_message);

    end_STUN_message(request_message);

    STUN_request(connection, request_message);
    destroy_string(request_message);

    response_message = STUN_response(connection);
    destroy_network_connection(connection);

    if(!response_message)
        goto error;

    attributes = create_STUN_attributes_from_message(response_message);

    destroy_network_connection(connection);
    destroy_STUN_attributes(attributes);

    return 1;

error:
    return 0;
}
