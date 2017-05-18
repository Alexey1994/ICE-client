#include "TURN.h"
#include "../../data structures/string/string.h"
#include "../../logger/logger.h"

#include "debug.h"

#include "attributes/DATA.c"

#include "attribute reader.c"


void initialize_TURN()
{
    read_attribute_TURN_handlers[DATA_TURN_ATTRIBUTE] = read_DATA_attribute;

#if ENABLE_TURN_DEBUG
    initialize_TURN_debug();
#endif
}


static void TURN_request(NetworkConnection connection, String *message)
{
    write_in_network_connection(connection, message->begin, message->length);

#if ENABLE_STUN_DEBUG
    print_TURN_request(message);
#endif
}


static void TURN_response_handler(Byte *data, Byte *end_response)
{
    if(!data)
    {
        *end_response = TIMEOUT_ERROR;
        return;
    }

    *end_response = 1;
}


String* TURN_response(NetworkConnection connection)
{
    STUN_Head *head;
    Byte       end_response;

    String *message = create_string(200);

    async_read_from_network_connection(connection, 500, message->begin, 200, TURN_response_handler, &end_response);

    while(!end_response);// waiting

    if(end_response == TIMEOUT_ERROR)
        goto error;

    head = message->begin;
    convert_big_to_little_endian(&head->content_length, 2);
    message->length = 20 + head->content_length;

    return message;

error:
    destroy_string(message);
    return 0;
}


TURN_Attributes* create_TURN_attributes_from_message(String *message)
{
    TURN_Attributes *attributes  = new(TURN_Attributes);

    attributes->STUN_attributes = create_STUN_attributes();

    read_TURN_attributes(attributes, message);

    return attributes;
}


void destroy_TURN_attributes(TURN_Attributes *attributes)
{
    free(attributes);
}


void TURN(char *host, short port)
{
    NetworkConnection  connection  =  create_UDP_connection(host, port);

    if(!connection)
        goto error;

    String *request_message = create_STUN_head(ALLOCATE_TURN_MESSAGE);
    set_STUN_content_length(request_message->begin, 0);

    TURN_request(connection, request_message);

    String *response_message = TURN_response(connection);
    destroy_network_connection(connection);

    if(!response_message)
        goto error;

    create_TURN_attributes_from_message(response_message);

#if ENABLE_TURN_DEBUG
    print_TURN_response(response_message);
#endif

    return;

error:
    return;
}