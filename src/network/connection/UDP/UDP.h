#ifndef UDP_NETWORK_CONNECTION
#define UDP_NETWORK_CONNECTION


#include "../../../error/error.h"


#define NO_ERRORS     0
#define TIMEOUT_ERROR 1


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