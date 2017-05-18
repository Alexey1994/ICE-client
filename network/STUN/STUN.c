#include "../network info/network info.h"
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


static void(*read_attribute_handlers[65536])(STUN_Attributes *attributes, Byte *attribute, int attribute_length);


void initialize_STUN()
{
    int i;

    for(i=0; i<65536; ++i)
        read_attribute_handlers[i] = 0;

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


void destroy_attributes(STUN_Attributes *attributes)
{
    //free(attributes->MAPPRED_ADDRESS.host);
    free(attributes);
}


static void read_attribute(STUN_Attribute *attribute, STUN_Attributes *attributes)
{
    void(*attribute_handler)(STUN_Attributes *attributes, Byte *attribute, int attribute_length);

    attribute_handler = read_attribute_handlers[ attribute->type ];

    if(!attribute_handler)
        return;

    attribute_handler(attributes, (Byte*)attribute + 4, attribute->length);
}


static void read_attributes(STUN_Attributes *attributes, String *message)
{
    int             length     = 20;
    STUN_Attribute *attribute  = message->begin + 20;

    while(length < message->length)
    {
        convert_big_to_little_endian(&attribute->type, 2);
        convert_big_to_little_endian(&attribute->length, 2);
        read_attribute(attribute, attributes);
        length += 4 + attribute->length;
        attribute = (Byte*)attribute + 4 + attribute->length;
    }
}


static void generate_transaction_ID(Byte *transaction_ID)
{
    memset(transaction_ID, 0, 12);
}


static void add_request_head(String *message, int content_length)
{
    STUN_Header *header      =  message->begin;

    header->message_type   = BINDING_REQUEST;
    header->message_length = content_length;
    header->magic_cookie   = STUN_COOKIE;
    generate_transaction_ID(header->transaction_ID);

    message->length = 20;
}


void add_attribute_head(Stream *attributes, unsigned short type, unsigned short length)
{
    convert_big_to_little_endian(&type, 2);
    convert_big_to_little_endian(&length, 2);

    write_data_in_output_stream(attributes, &type, 2);
    write_data_in_output_stream(attributes, &length, 2);
}


void request(NetworkConnection connection, String *message)
{
    write_in_network_connection(connection, message->begin, 20);
}


static void response_handler(Byte *data, Byte *end_response)
{
    if(!data)
    {
        *end_response = TIMEOUT_ERROR;
        return;
    }

    *end_response = 1;
}


String* response(NetworkConnection connection)//, STUN_Attributes *attributes)
{
    STUN_Header *header;
    Byte         end_response;

    String      *message = create_string(200);

    async_read_from_network_connection(connection, 500, message->begin, 200, response_handler, &end_response);

    while(!end_response);// waiting

    if(end_response == TIMEOUT_ERROR)
        goto error;

    header = message->begin;
    convert_big_to_little_endian(&header->message_length, 2);
    message->length = 20 + header->message_length;

    return message;

error:
    destroy_string(message);
    return 0;
}


STUN_Attributes* STUN_request(char *host, int port)
{
    String *response_message;

    NetworkConnection  connection  =  create_UDP_connection(host, port);

    if(!connection)
        goto error;

    STUN_Attributes   *attributes        = new(STUN_Attributes);
    String            *head              = create_string(20);
    String            *source_attributes = create_string(10);

    attributes->MAPPRED_ADDRESS.host = 0;
    attributes->CHANGED_ADDRESS.host = 0;

    add_request_head(head, source_attributes->length);
    concatenate_strings(head, source_attributes);
    destroy_string(source_attributes);

    request(connection, head);

#if ENABLE_STUN_DEBUG
    print_STUN_request(head);
#endif

    response_message = response(connection);

    if(!response_message)
        goto error;

    read_attributes(attributes, response_message);

#if ENABLE_STUN_DEBUG
    print_STUN_response(response_message);
#endif
    destroy_string(response_message);

    return attributes;

error:
    return 0;
}


void get_NAT_type_using_STUN_server(char *host, unsigned short port)
{
    char             local_address[16];
    STUN_Attributes *attributes2;
    STUN_Attributes *attributes1 = STUN_request(host, port);

    get_IPv4_host_address(local_address);
    printf("local address: %s\n", local_address);

    if(!attributes1)
    {
        printf("NAT + Firewall or connection error\n");
        return 0;
    }

    printf("mapped address1: %s:%d\n", attributes1->MAPPRED_ADDRESS.host, attributes1->MAPPRED_ADDRESS.port);

    if(!strcmp(attributes1->MAPPRED_ADDRESS.host, local_address))
    {
        printf("CONE NAT\n");
    }

    if(!attributes1->CHANGED_ADDRESS.host)
    {
        attributes1->CHANGED_ADDRESS.host = host;
        attributes1->CHANGED_ADDRESS.port = 3479;
    }

    printf("changed address1: %s:%d\n", attributes1->CHANGED_ADDRESS.host, attributes1->CHANGED_ADDRESS.port);
}
