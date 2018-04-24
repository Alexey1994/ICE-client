#ifndef UDP_NETWORK_CONNECTION_H_INCLUDED
#define UDP_NETWORK_CONNECTION_H_INCLUDED


#include "../connection.h"


typedef struct
{
    int      socket;

    Address *source;
    Address *destination;

    N_32     maximum_waiting_time;
    N_32     current_time;
}
UDP_Connection;

internal function  Boolean initialize_UDP_Connection      (UDP_Connection *connection, Address *destination, N_32 maximum_waiting_time);
internal procedure         deinitialize_UDP_Connection    (UDP_Connection *connection);

internal function  Boolean set_UDP_Connection_source      (UDP_Connection *connection, Address *source);
internal procedure         set_UDP_Connection_destination (UDP_Connection *connection, Address *destination);

internal function  Boolean read_from_UDP_Connection       (UDP_Connection *connection, Byte *data, N_32 *length);
internal function  Boolean write_in_UDP_Connection        (UDP_Connection *connection, Byte *data, N_32 length);


#include "UDP.c"

#endif //UDP_NETWORK_CONNECTION_H_INCLUDED