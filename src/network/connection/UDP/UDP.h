#ifndef UDP_NETWORK_CONNECTION
#define UDP_NETWORK_CONNECTION


typedef struct
{
	int   socket;
	Byte *address;
}
UDP_Connection;


UDP_Connection* create_UDP    (Byte *host, int port);
void            destroy_UDP   (UDP_Connection *connection);

void            write_in_UDP  (UDP_Connection *connection, Byte *data, int length_data);
void            read_from_UDP (UDP_Connection *connection, Byte *data, int length_data);


#include "UDP.c"

#endif //UDP_NETWORK_CONNECTION