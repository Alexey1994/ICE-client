#include "TURN.h"
#include "../../data structures/string/string.h"

#include "debug.h"

#include "attribute reader.c"
#include "request.c"
#include "response.c"

#include "attributes/DATA.c"
#include "attributes/REQUESTED_TRANSPORT.c"
#include "attributes/DONT_FRAGMENT.c"
#include "attributes/XOR_PEER_ADDRESS.c"


void initialize_TURN()
{
    read_attribute_TURN_handlers[DATA_TURN_ATTRIBUTE] = read_DATA_attribute;

#ifdef ENABLE_DEBUG
    initialize_TURN_debug();
#endif
}


void TURN(char *host, short port)
{
    NetworkConnection  connection  =  create_UDP_connection(host, port);

    if(!connection)
        goto error;

    TURN_Attributes *attributes;
    String          *request_message;

    begin_TURN_request(&request_message, ALLOCATE_TURN_MESSAGE);
        add_REQUESTED_TRANSPORT(request_message, UDP_CONNECTION);
        //add_DONT_FRAGMENT(request_message);
    end_TURN_request(connection, request_message);

    attributes = get_response_TURN_attributes(connection);
    destroy_network_connection(connection);

    if(!attributes)
        goto error;

    destroy_TURN_attributes(attributes);

    return;

error:
    return;
}


void send_TURN(char *host, short port)
{
    NetworkConnection  connection  =  create_UDP_connection(host, port);

    if(!connection)
        goto error;

    TURN_Attributes *attributes;
    String          *request_message;

    begin_TURN_request(&request_message, SEND_TURN_MESSAGE);
        add_REQUESTED_TRANSPORT(request_message, UDP_CONNECTION);
        //add_DONT_FRAGMENT(request_message);
    end_TURN_request(connection, request_message);

    attributes = get_response_TURN_attributes(connection);
    destroy_network_connection(connection);

    if(!attributes)
        goto error;

    destroy_TURN_attributes(attributes);

    return;

error:
    return;
}


void bind_TURN_channel(char *host, short port)
{
    NetworkConnection  connection  =  create_UDP_connection(host, port);

    if(!connection)
        goto error;

    TURN_Attributes *attributes;
    String          *request_message;

    begin_TURN_request(&request_message, CHANNEL_BIND_TURN_MESSAGE);
        add_XOR_PEER_ADDRESS(request_message);
        //add_REQUESTED_TRANSPORT(request_message, UDP_CONNECTION);
        //add_DONT_FRAGMENT(request_message);
    end_TURN_request(connection, request_message);

    attributes = get_response_TURN_attributes(connection);
    destroy_network_connection(connection);

    if(!attributes)
        goto error;

    destroy_TURN_attributes(attributes);

    return;

error:
    return;
}