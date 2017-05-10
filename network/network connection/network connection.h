#ifndef NETWORK_CONNECTION_H_INCLUDED
#define NETWORK_CONNECTION_H_INCLUDED


#include <libs/extends/extends.h>
#include <libs/stream/stream.h>


typedef unsigned int NetworkConnection;


NetworkConnection create_network_connection(Byte *host, int port, int connection_domain, int connection_type, int connection_protocol);
NetworkConnection create_TCP_connection(Byte *host, int port);
NetworkConnection create_UDP_connection(Byte *host, int port);
void              destroy_network_connection(NetworkConnection connection);

//void              create_TCP_server();

void              open_network_connection(NetworkConnection connection, Byte *path, unsigned int length_data);
Byte              get_TCP_byte(NetworkConnection connection);
void              get_network_connection_array(NetworkConnection connection, Byte *array, int length);


#endif // NETWORK_CONNECTION_H_INCLUDED
