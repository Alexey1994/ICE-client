#include "network connection.h"

#include "../../error/error.h"
#include "../../thread/thread.h"
#include <errno.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>


NetworkConnection create_network_connection(Byte *host, int port, int connection_domain, int connection_type, int connection_protocol)
{
    struct sockaddr_in  sock_addr;
    struct hostent     *host_data;
    NetworkConnection   socket_connection;

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

    fcntl(socket_connection, F_SETFL, O_NONBLOCK);
/*
    fcntl( socket_connection, F_SETFL, fcntl( socket_connection, F_GETFL ) | O_NONBLOCK );


    fd_set         fd;
    struct timeval tv;

    tv.tv_sec  = 1;
    tv.tv_usec = 0;

    if( select( socket_connection + 1, 0, &fd, 0, &tv ) == SOCKET_ERROR )
        goto error;
*/
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
    close(connection);
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

    int current_time = 0;

    for(;;)
    {
        recv(connection, data, length_data, 0);

        if(errno != EWOULDBLOCK)
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

/*
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
}*/