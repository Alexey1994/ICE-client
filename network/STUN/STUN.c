#include "STUN.h"
#include <libs/data structures/string/string.h>
#include <libs/logger/logger.h>

#include "debug.h"

#include "attributes/MAPPED_ADDRESS.h"
#include "attributes/RESPONSE_ADDRESS.h"
#include "attributes/CHANGE_REQUEST.h"
#include "attributes/SOURCE_ADDRESS.h"
#include "attributes/CHANGED_ADDRESS.h"
#include "attributes/USERNAME.h"
#include "attributes/PASSWORD.h"
#include "attributes/MESSAGE_INTEGRITY.h"
#include "attributes/ERROR_CODE.h"
#include "attributes/UNKNOWN_ATTRIBUTES.h"
#include "attributes/REFLECTED_FROM.h"
#include "attributes/REALM.h"
#include "attributes/NONCE.h"
#include "attributes/XOR_MAPPED_ADDRESS.h"
#include "attributes/SOFTWARE.h"
#include "attributes/ALTERNATE_SERVER.h"
#include "attributes/FINGERPRINT.h"


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

    #ifdef __WIN32__
    if(!wsa_data)
    {
        wsa_data = new(WSADATA);
        WSAStartup(MAKEWORD(2, 2), wsa_data);
    }
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


static Byte verify_head(STUN_Header *header)
{
    switch(header->message_type)
    {
        case BINDING_REQUEST:        break;
        case BINDING_RESPONSE:       break;
        case BINDING_ERROR_RESPONSE: break;

        default:
            goto error;
    }

    return 1;

error:
    return 0;
}


static char request(NetworkConnection connection, String *message)
{
    if(!verify_head(message->begin))
        goto error;

    #if ENABLE_STUN_DEBUG
    print_STUN_request(message);
    #endif

    write_in_network_connection(connection, message->begin, 20);

    return 1;

error:
    return 0;
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


static Byte response(NetworkConnection connection, STUN_Attributes *attributes)
{
    STUN_Header *header;
    Byte         end_response;

    String      *message = create_string(200);

    //sync_read_from_network_connection(connection, message->begin, 200);
    async_read_from_network_connection(connection, 500, message->begin, 200, response_handler, &end_response);

    while(!end_response);// waiting

    if(end_response == TIMEOUT_ERROR)
        goto error;

    header = message->begin;
    convert_big_to_little_endian(&header->message_length, 2);
    message->length = 20 + header->message_length;

    if(!verify_head(header))
        goto error;

    read_attributes(attributes, message);
    #if ENABLE_STUN_DEBUG
    print_STUN_response(message);
    #endif

    return 1;

error:
    return 0;
}


STUN_Attributes* STUN_request(char *host, int port)
{
    NetworkConnection  connection  =  create_TCP_connection(host, port);;//create_UDP_connection(host, port);

    if(!connection)
        goto error;

    STUN_Attributes   *attributes        = new(STUN_Attributes);
    String            *head              = create_string(20);
    String            *source_attributes = create_string(10);
    //Stream            *attributes_stream = create_output_stream(source_attributes, push_in_string);

    //add_CHANGED_ADDRESS_attribute(attributes_stream, 4, "127.0.0.1", 80);
    add_request_head(head, source_attributes->length);
    concatenate_strings(head, source_attributes);
    destroy_string(source_attributes);
    //destroy_stream(attributes_stream);
    //add_CHANGED_ADDRESS_request_header(message);

    if(!request(connection, head))
        goto error;

    if(!response(connection, attributes))
        goto error;

    return attributes;

error:
    return 0;
}

/*
STUN_Attributes* get_STUN_attributes(char *STUN_host, unsigned short STUN_port)
{
    STUN_Attributes *attributes = STUN_request(STUN_host, STUN_port);

    if(!attributes)
        goto error;

    return attributes;

error:
    return 0;
}*/


void get_NAT_type_using_STUN_server(char *host, unsigned short port)
{
    /*
    char            mapped_host1[16];
    unsigned short  mapped_port1;

    char            changed_host[16];
    unsigned short  changed_port;

    char            computer_address[16];

    if(!get_STUN_MAPPED_ADDRESS(host, port, mapped_host1, &mapped_port1))
        return 0;

    printf("mapped address1: %s:%d\n", mapped_host1, mapped_port1);

    //if(!get_STUN_CHANGED_ADDRESS())

    get_IPv4_host_address(computer_address);
    printf("computer address: %s\n", computer_address);

    if(!strcmp(mapped_host1, computer_address))
    {
        printf("CONE NAT\n");
    }*/

    char             computer_address[16];
    STUN_Attributes *attributes2;
    STUN_Attributes *attributes1 = STUN_request(host, port);

    if(!attributes1)
    {
        printf("NAT + Firewall or connection error\n");
        return 0;
    }

    printf("mapped address1: %s:%d\n", attributes1->MAPPRED_ADDRESS.host, attributes1->MAPPRED_ADDRESS.port);

    get_IPv4_host_address(computer_address);

    if(!strcmp(attributes1->MAPPRED_ADDRESS.host, computer_address))
    {
        printf("CONE NAT\n");
    }

    if(!attributes1->CHANGED_ADDRESS.host)
    {
        attributes1->CHANGED_ADDRESS.host = host;
        attributes1->CHANGED_ADDRESS.port = 3479;
    }

    printf("changed address1: %s:%d\n", attributes1->CHANGED_ADDRESS.host, attributes1->CHANGED_ADDRESS.port);


    //attributes2 = get_STUN_attributes(attributes1->CHANGED_ADDRESS.host, attributes1->CHANGED_ADDRESS.port);


}
