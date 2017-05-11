#ifndef NETWORK_CONNECTION_H_INCLUDED
#define NETWORK_CONNECTION_H_INCLUDED


#include "../../extends/extends.h"


typedef unsigned int NetworkConnection;


NetworkConnection create_network_connection(Byte *host, int port, int connection_domain, int connection_type, int connection_protocol);
NetworkConnection create_TCP_connection(Byte *host, int port);
NetworkConnection create_UDP_connection(Byte *host, int port);
void              destroy_network_connection(NetworkConnection connection);

//void              create_TCP_server();

void              write_in_network_connection(NetworkConnection connection, Byte *data, unsigned int length_data);
Byte              get_TCP_byte(NetworkConnection connection);

void              sync_read_from_network_connection(NetworkConnection connection, Byte *data, int length_data);
void              async_read_from_network_connection(NetworkConnection connection, int timeout, Byte *data, int length_data, void handler(Byte *data, Byte *arguments), Byte *handler_arguments);


#endif // NETWORK_CONNECTION_H_INCLUDED
