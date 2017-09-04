#ifndef NETWORK_SERVER_H_INCLUDED
#define NETWORK_SERVER_H_INCLUDED


#include "../../error/error.h"
#include "../../thread/thread.h"


typedef int Network_Source;


typedef struct
{
    Network_Source      source;
    Byte               *address;
    //struct sockaddr_in  sock_addr;
}
Server;


Server* create_UDP_server(
    Byte           *host,
    unsigned short  port,
    int           (*run_on_request)(Network_Source source, Byte *arguments),
    Byte           *request_arguments
);


Server* create_TCP_server(
    Byte           *host,
    unsigned short  port,
    int           (*run_on_request)(Network_Source source, Byte *arguments),
    Byte           *request_arguments
);


void destroy_server(Server *server);


#include "network server.c"

#endif //NETWORK_SERVER_H_INCLUDED