#include "TURN.h"
#include "../../data structures/string/string.h"

#include "debug.h"

#include "attribute reader.c"
#include "request.c"
#include "response.c"

#include "attributes/DATA.c"
#include "attributes/LIFETIME.c"
#include "attributes/REQUESTED_TRANSPORT.c"
#include "attributes/DONT_FRAGMENT.c"
#include "attributes/XOR_PEER_ADDRESS.c"
#include "attributes/XOR_RELAYED_ADDRESS.c"


void initialize_TURN()
{
    read_attribute_TURN_handlers[DATA_TURN_ATTRIBUTE]                = read_DATA_attribute;
    read_attribute_TURN_handlers[XOR_RELAYED_ADDRESS_TURN_ATTRIBUTE] = read_XOR_RELAYED_ADDRESS_attribute;

#ifdef ENABLE_DEBUG
    initialize_TURN_debug();
#endif
}


void allocate_TURN(char *host, short port, N_32 *xor_relayed_host, N_16 *xor_relayed_port)
{
    UDP_Connection *connection = create_UDP(host, port);

    if(!connection)
        goto error;

    TURN_Attributes *attributes;
    String          *request_message;

    begin_TURN_request(&request_message, ALLOCATE_TURN_MESSAGE);
        add_REQUESTED_TRANSPORT(request_message, TCP_CONNECTION);
        //add_USERNAME(request_message, "Lex");
        //add_MESSAGE_INTEGRITY(request_message);
        //add_LIFETIME(request_message, 2);
        //add_DONT_FRAGMENT(request_message);
    end_TURN_request(connection, request_message);

    attributes = get_response_TURN_attributes(connection);
    destroy_UDP(connection);

    if(!attributes)
        goto error;

    *xor_relayed_host = attributes->XOR_RELAYED_ADDRESS.host;
    *xor_relayed_port = attributes->XOR_RELAYED_ADDRESS.port;

    destroy_TURN_attributes(attributes);

    return;

error:
    return;
}


void create_TURN_permission(char *host, short port, N_32 permissions_host, N_16 permissions_port)
{
    UDP_Connection *connection = create_UDP(host, port);

    if(!connection)
        goto error;

    TURN_Attributes *attributes;
    String          *request_message;

    begin_TURN_request(&request_message, CREATE_PERMISSION_TURN_MESSAGE);
        add_XOR_PEER_ADDRESS(request_message, permissions_host, permissions_port);
        add_USERNAME(request_message, "lexey");
    end_TURN_request(connection, request_message);

    attributes = get_response_TURN_attributes(connection);
    destroy_UDP(connection);

    if(!attributes)
        goto error;

    destroy_TURN_attributes(attributes);

    return;

error:
    return;
}


void send_TURN_data(char *host, short port)
{
    UDP_Connection *connection = create_UDP(host, port);

    if(!connection)
        goto error;

    TURN_Attributes *attributes;
    String          *request_message;

    begin_TURN_request(&request_message, SEND_TURN_MESSAGE);
        //add_REQUESTED_TRANSPORT(request_message, UDP_CONNECTION);
        //add_DONT_FRAGMENT(request_message);
    end_TURN_request(connection, request_message);

    attributes = get_response_TURN_attributes(connection);
    destroy_UDP(connection);

    if(!attributes)
        goto error;

    destroy_TURN_attributes(attributes);

    return;

error:
    return;
}


void receive_TURN_data(char *host, short port)
{
    UDP_Connection *connection = create_UDP(host, port);

    if(!connection)
        goto error;

    TURN_Attributes *attributes;
    String          *request_message;

    begin_TURN_request(&request_message, DATA_TURN_MESSAGE);
        //add_XOR_PEER_ADDRESS(request_message);
        //add_REQUESTED_TRANSPORT(request_message, UDP_CONNECTION);
        //add_DONT_FRAGMENT(request_message);
    end_TURN_request(connection, request_message);

    attributes = get_response_TURN_attributes(connection);
    destroy_UDP(connection);

    if(!attributes)
        goto error;

    destroy_TURN_attributes(attributes);

    return;

error:
    return;
}


void bind_TURN_channel(char *host, short port)
{
    UDP_Connection *connection = create_UDP(host, port);

    if(!connection)
        goto error;

    TURN_Attributes *attributes;
    String          *request_message;

    begin_TURN_request(&request_message, CHANNEL_BIND_TURN_MESSAGE);
        //add_XOR_PEER_ADDRESS(request_message);
        //add_REQUESTED_TRANSPORT(request_message, UDP_CONNECTION);
        //add_DONT_FRAGMENT(request_message);
    end_TURN_request(connection, request_message);

    attributes = get_response_TURN_attributes(connection);
    destroy_UDP(connection);

    if(!attributes)
        goto error;

    destroy_TURN_attributes(attributes);

    return;

error:
    return;
}
