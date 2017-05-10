#include "network connection.h"

#include <libs/error/error.h>

#ifdef __WIN32__
# include <WinSock2.h>
# include <WS2tcpip.h>

static WSADATA *wsa_data = 0;

#endif

#ifdef __ANDROID_API__
# include <sys/socket.h>
# include <netinet/in.h>
# include <netdb.h>
# include <arpa/inet.h>
#endif // __ANDROID_API__


NetworkConnection create_network_connection(Byte *host, int port, int connection_domain, int connection_type, int connection_protocol)
{
    struct sockaddr_in  sock_addr;
    struct hostent     *host_data;
    NetworkConnection   socket_connection;

    #ifdef __WIN32__
    if(!wsa_data)
    {
        wsa_data=new(WSADATA);
        WSAStartup(MAKEWORD(2, 2), wsa_data);
    }
    #endif // __WIN32__

    host_data = gethostbyname(host);

    if(!host_data)
    {
        print_error("host name not correct\n");
        goto error;
    }

    sock_addr.sin_port        = htons(port);
    sock_addr.sin_family      = AF_INET;
    sock_addr.sin_addr.s_addr = *((unsigned long*)host_data->h_addr);

    socket_connection = socket(connection_domain, connection_type, connection_protocol);

    if (socket_connection == -1)
    {
        print_error("connection error\n");
        goto error;
    }

    if (connect(socket_connection, &sock_addr, sizeof(sock_addr)))
        goto error;

    return socket_connection;

error:
    print_error("connection not found\n");

    return 0;
}


NetworkConnection create_TCP_connection(Byte *host, int port)
{
    return create_network_connection(host, port, AF_INET, SOCK_STREAM, IPPROTO_TCP);
}


NetworkConnection create_UDP_connection(Byte *host, int port)
{
    return create_network_connection(host, port, AF_INET, SOCK_DGRAM, IPPROTO_UDP);
}


void destroy_network_connection(NetworkConnection connection)
{
    //closesocket(connection);
    //WSACleanup();
}


void open_network_connection(NetworkConnection connection, Byte *path, unsigned int length_data)
{
    int  count_data;
    Byte byte;

    send(connection, path, length_data, 0);
}


Byte get_TCP_byte(NetworkConnection connection)
{
    Byte byte;

    recv(connection, &byte, 1, 0);

    return byte;
}


void get_network_connection_array(NetworkConnection connection, Byte *array, int length)
{
    recv(connection, array, length, 0);
}

/*
NetworkConnection create_network_server(Byte *host, int port, int connection_domain, int connection_type, int connection_protocol)
{

}*/

/*
void create_TCP_server()
{char *host="localhost";
int port=8880;
    SOCKET mysocket;
    struct hostent *host_data;

    wsa_data=new(WSADATA);
    WSAStartup(MAKEWORD(2, 2), wsa_data);

    mysocket = socket(AF_INET, SOCK_STREAM, 0);

    host_data = gethostbyname(host);

    if(!host_data)
    {
        print_error("host name not correct\n");
        goto error;
    }

    SOCKADDR_IN local_addr;
    local_addr.sin_family      = AF_INET;
    local_addr.sin_port        = htons(port);
    local_addr.sin_addr.s_addr = 0;

    if (bind(mysocket, &local_addr, sizeof(local_addr)))
    {
        print_error("error binding \nhost: ");
        print_error(host);
        print_error("\n");

        goto error;
        closesocket(mysocket); // ��������� �����!
        //WSACleanup();
    }

    if (listen(mysocket, 0x100))
    {
        print_error("error listen\n");
        closesocket(mysocket);
        //WSACleanup();
    }


    SOCKADDR_IN client_addr;
    int client_addr_size = sizeof(client_addr);
    SOCKET client_socket;

    char *s=//"HTTP/1.1 200 OK\r\n"
            //"Server: Apache\r\n"
            //"Content-Type: text/html; charset=utf-8\r\n"
            //"Content-Length: 1\r\n"
            "Connection: close\r\n"
            "\r\n"
            "Hi";

    client_socket=accept(mysocket, &client_addr, &client_addr_size);

    send(client_socket, s, strlen(s), 0);

    char byte;
    int i;
    loop
    {
        i=recv(client_socket, &byte, 1, 0);
printf("%d", i);
        if(i<=0)
            break;

        printf("%c", byte);
    }
printf("Ok");
    //send(client_socket, &byte, 1, 0);

    //send(client_socket, s, strlen(s), 0);

error:
    return;
}
*/
