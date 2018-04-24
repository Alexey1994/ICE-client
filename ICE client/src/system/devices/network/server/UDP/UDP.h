#ifndef UDP_NETWORK_SERVER_H_INCLUDED
#define UDP_NETWORK_SERVER_H_INCLUDED


#include "../../connection/UDP/UDP.h"
#include "../../../../interfaces/thread/thread.h"


#define UDP_SERVER_RECEIVE_BUFFER_SIZE 1536


typedef struct
{
    int      socket;

    Address *source;
    Address *destination;
    Byte    *attributes;
    Boolean  server_is_running;

    function Boolean (*on_request) (Address *client_address, Byte *data, N_32 data_length, Byte *attributes);
}
UDP_Server;


internal function  Boolean initialize_UDP_Server   (UDP_Server *server, Address *server_address, function Boolean (*on_request) (Address *client_address, Byte *data, N_32 data_length, Byte *attributes), Byte *attributes);
internal procedure         deinitailize_UDP_Server (UDP_Server *server);


#include "UDP.c"

#endif //UDP_NETWORK_SERVER_H_INCLUDED