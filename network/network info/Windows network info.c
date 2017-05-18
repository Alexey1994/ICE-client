#include <WinSock2.h>
#include <WS2tcpip.h>

#include "../network connection/network connection.h"
#include "../../extends/extends.h"
#include "../../error/error.h"
#include "../../system library/system library.h"


#ifdef __WIN32__
//static WSADATA *wsa_data = 0;
#endif


static int get_host_name(char *name, int name_length)
{
    #ifdef __WIN32__
    if(!wsa_data)
    {
        wsa_data = new(WSADATA);
        WSAStartup(MAKEWORD(2, 2), wsa_data);
    }
    #endif

    return gethostname(name, name_length);
}


static int get_address_info(char *node_name, char *service_name, struct addrinfo *hints, struct addrinfo **result)
{
    #ifdef __WIN32__
    static int (*get_address_info)(char             *node_name,
                                   char             *service_name,
                                   struct addrinfo  *hints,
                                   struct addrinfo **result) = 0;

    if(!get_address_info)
        get_address_info = open_system_function("ws2_32.dll", "getaddrinfo");

    return get_address_info(node_name, service_name, hints, result);
    #endif //__WIN32__
}


void get_IPv4_host_address(char *address)
{
    Byte *host;

    struct hostent *lh     = gethostbyname(0);

    host = lh->h_addr_list[0];

    if(!host)
    {
        snprintf(address, 16, "127.0.0.1");
        return;
    }

    snprintf(address, 16, "%u.%u.%u.%u\n", host[0], host[1], host[2], host[3]);
}
