#include "../../stream/stream.h"
#include "../../data structures/string/string.h"

#include "debug.h"
//#include "../network connection/network connection.h"

#include "head.c"
#include "attribute reader.c"
#include "request.c"
#include "response.c"

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

#ifdef ENABLE_DEBUG
    initialize_STUN_debug();
#endif
}


Boolean get_STUN_mapped_address(char *host, unsigned short port, char *mapped_host, unsigned short *mapped_port)
{
    UDP_Connection *connection  =  create_UDP(host, port);
printf("get stun mapped address\n");
    if(!connection)
        goto error;

    STUN_Attributes *attributes;
    String          *request_message;

    begin_STUN_request(&request_message, BINDING_REQUEST);
    end_STUN_request(connection, request_message);

    attributes = get_response_STUN_attributes(connection);
    destroy_UDP(connection);

    if(!attributes)
        goto error;

    strcpy(mapped_host, attributes->MAPPED_ADDRESS.host);
    *mapped_port = attributes->MAPPED_ADDRESS.port;

    destroy_STUN_attributes(attributes);

    return 1;

error:
    return 0;
}


Boolean authenticate_on_STUN_server(char *host, unsigned short port)
{
    UDP_Connection *connection  =  create_UDP(host, port);

    if(!connection)
        goto error;

    STUN_Attributes *attributes;
    String          *request_message;

    begin_STUN_request(&request_message, BINDING_REQUEST);
        add_USERNAME(request_message, "lex");
        add_NONCE(request_message, "2e131a5fb210812c");
        add_REALM(request_message);
        add_MESSAGE_INTEGRITY(request_message);
    end_STUN_request(connection, request_message);

    attributes = get_response_STUN_attributes(connection);
    destroy_UDP(connection);

    if(!attributes)
        goto error;

    destroy_STUN_attributes(attributes);

    return 1;

error:
    return 0;
}
