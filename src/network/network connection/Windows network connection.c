#include "network connection.h"

#include "../../thread/thread.h"
#include "../../error/error.h"
#include <stdio.h>
#include <errno.h>
#include <WinSock2.h>
#include <WS2tcpip.h>


static WSADATA *wsa_data = 0;
/*

NetworkConnection create_TCP_connection(Byte *host, int port)
{
    struct sockaddr_in  sock_addr;
    NetworkConnection   socket_connection;

    if(!wsa_data)
    {
        wsa_data=new(WSADATA);
        WSAStartup(MAKEWORD(2, 2), wsa_data);
    }

    sock_addr.sin_port        = htons(port);
    sock_addr.sin_family      = AF_INET;
    sock_addr.sin_addr.s_addr = inet_addr(host);

    socket_connection = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (socket_connection == -1)
    {
        print_error("connection error\n");
        goto error;
    }

    unsigned non_blocking = 1;
    ioctlsocket(socket_connection, FIONBIO, &non_blocking);

    //while(connect(socket_connection, &sock_addr, sizeof(sock_addr)) == EINPROGRESS);
    while(!connect(socket_connection, &sock_addr, sizeof(sock_addr)));

    return socket_connection;

error:
    print_error("connection not found\n");

    return 0;
}


Network_Connection* create_UDP_connection(Byte *host, int port)
{
    struct sockaddr_in *sock_addr;
    Network_Connection *connection;

    if(!wsa_data)
    {
        wsa_data=new(WSADATA);
        WSAStartup(MAKEWORD(2, 2), wsa_data);
    }

    sock_addr = new(struct sockaddr_in);

    sock_addr->sin_family      = AF_INET;
    sock_addr->sin_addr.s_addr = inet_addr(host);
    sock_addr->sin_port        = htons(port);

    connection = new(Network_Connection);

    connection->address = sock_addr;
    connection->connection = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (connection->connection == -1)
    {
        print_error("connection error\n");
        goto error;
    }

    //unsigned non_blocking = 1;
    //ioctlsocket(connection->connection, FIONBIO, &non_blocking);

    //while(connect(socket_connection, &sock_addr, sizeof(sock_addr)) == EINPROGRESS);

    //while(!connect(socket_connection, &sock_addr, sizeof(sock_addr)))
    //connect(socket_connection, &sock_addr, sizeof(sock_addr));

    return connection;

error:
    print_error("connection not found\n");

    return 0;
}


void destroy_network_connection(NetworkConnection connection)
{
    closesocket(connection);
    //WSACleanup();
}



void write_in_network_connection(NetworkConnection connection, Byte *data, unsigned int length_data)
{
    send(connection, data, length_data, 0);
}


void sync_read_from_network_connection(NetworkConnection connection, Byte *data, int length_data)
{
    recv(connection, data, length_data, 0);
}


typedef struct
{
    NetworkConnection  connection;
    int                timeout;
    Byte              *data;
    int                length_data;
    void             (*handler)(Byte *data, Byte *arguments);
    Byte              *handler_arguments;
}
NetworkConnection_Read_Arguments;


void update_network_connection_read_thread(NetworkConnection_Read_Arguments *arguments)
{
    NetworkConnection  connection                            = arguments->connection;
    int                timeout                               = arguments->timeout;
    Byte              *data                                  = arguments->data;
    int                length_data                           = arguments->length_data;
    void             (*handler)(Byte *data, Byte *arguments) = arguments->handler;
    Byte              *handler_arguments                     = arguments->handler_arguments;
    free(arguments);

    int      current_time = 0;
    int      error_code;

    for(;;)
    {
        recv(connection, data, length_data, 0);
        error_code = WSAGetLastError();

        if(error_code != WSAEWOULDBLOCK)
            break;

        if(current_time > timeout)
        {
            handler(0, handler_arguments);
            return;
        }

        sleep_thread(10);
        current_time += 10;
    }

    handler(data, handler_arguments);
}


void async_read_from_network_connection(NetworkConnection connection, int timeout, Byte *data, int length_data, void handler(Byte *data, Byte *arguments), Byte *handler_arguments)
{
    NetworkConnection_Read_Arguments *arguments = new(NetworkConnection_Read_Arguments);

    arguments->connection        = connection;
    arguments->timeout           = timeout;
    arguments->data              = data;
    arguments->length_data       = length_data;
    arguments->handler           = handler;
    arguments->handler_arguments = handler_arguments;

    run_thread(update_network_connection_read_thread, arguments);
}


Byte get_TCP_byte(NetworkConnection connection)
{
    Byte byte;

    recv(connection, &byte, 1, 0);

    return byte;
}
*/