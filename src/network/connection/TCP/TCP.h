#ifndef TCP_NETWORK_CONNECTION
#define TCP_NETWORK_CONNECTION


#include "../../../error/error.h"


#define NO_ERRORS     0
#define TIMEOUT_ERROR 1


typedef struct
{
	int   socket;
	Byte *address;
}
TCP_Connection;


TCP_Connection* create_TCP    (Byte *host, int port);
void            destroy_TCP   (TCP_Connection *connection);

void            write_in_TCP  (TCP_Connection *connection, Byte *data, int length_data);
void            read_from_TCP (TCP_Connection *connection, Byte *data, int length_data);


#include "TCP.c"

#endif //TCP_NETWORK_CONNECTION