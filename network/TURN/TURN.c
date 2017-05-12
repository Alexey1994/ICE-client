#include "TURN.h"
#include "../../data structures/string/string.h"
#include "../../logger/logger.h"

#include "debug.h"

#include "attributes/DATA.h"


//static void(*read_attribute_handlers[65536])(STUN_Attributes *attributes, Byte *attribute, int attribute_length);


void initialize_TURN()
{

}


static TURN_Attributes* TURN_request(NetworkConnection connection)
{
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

    if(!response(connection))
        goto error;

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

    return TURN_request(connection);

error:
    return 0;
}