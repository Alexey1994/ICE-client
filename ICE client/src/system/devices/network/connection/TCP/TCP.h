#ifndef TCP_NETWORK_CONNECTION_H_INCLUDED
#define TCP_NETWORK_CONNECTION_H_INCLUDED


#include "../connection.h"


typedef struct
{
    int  socket;
    N_32 maximum_waiting_time;
    N_32 current_time;
}
TCP_Connection;


internal function  Boolean initialize_TCP_Connection   (TCP_Connection *connection, Address *destination, N_32 maximum_waiting_time);
internal procedure         deinitialize_TCP_Connection (TCP_Connection *connection);

internal function  Boolean read_from_TCP_Connection    (TCP_Connection *connection, Byte *data, N_32 length);
internal function  Boolean write_in_TCP_Connection     (TCP_Connection *connection, Byte *data, N_32 length);


#include "TCP.c"

#endif //TCP_NETWORK_CONNECTION_H_INCLUDED