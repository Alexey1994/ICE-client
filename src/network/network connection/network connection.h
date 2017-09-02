#ifndef NETWORK_CONNECTION_H_INCLUDED
#define NETWORK_CONNECTION_H_INCLUDED


#include "../../extends/extends.h"


typedef enum
{
	TIMEOUT_ERROR = 1,
    NO_ERRORS     = 100
}NetworkConnectionErrors;


typedef enum
{
	UDP_CONNECTION,
	TCP_CONNECTION	
}
ConnectionType;


typedef unsigned int NetworkConnection;

typedef struct
{
	NetworkConnection  connection;
	Byte              *address;
}
Network_Connection;


//NetworkConnection create_network_connection(Byte *host, int port, int connection_domain, int connection_type, int connection_protocol);
NetworkConnection create_TCP_connection(Byte *host, int port);
//UDP_Connection* create_UDP_connection(Byte *host, int port);
void              destroy_network_connection(NetworkConnection connection);

//void              create_TCP_server();

void              write_in_network_connection(NetworkConnection connection, Byte *data, unsigned int length_data);
Byte              get_TCP_byte(NetworkConnection connection);

void              sync_read_from_network_connection(NetworkConnection connection, Byte *data, int length_data);
void              async_read_from_network_connection(NetworkConnection connection, int timeout, Byte *data, int length_data, void handler(Byte *data, Byte *arguments), Byte *handler_arguments);


//void write_in_UDP(Network_Connection *connection, Byte *data, unsigned int length_data);

#include "network connection.c"

#endif // NETWORK_CONNECTION_H_INCLUDED
