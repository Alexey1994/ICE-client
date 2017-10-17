#include <stdio.h>
#include <errno.h>
#include <WinSock2.h>
#include <WS2tcpip.h>


//static WSADATA *wsa_data = 0;


TCP_Connection* create_TCP (Byte *host, int port)
{
	struct sockaddr_in *sock_addr;
    TCP_Connection     *connection;

    if(!wsa_data)
    {
        wsa_data=new(WSADATA);
        WSAStartup(MAKEWORD(2, 2), wsa_data);
    }

    sock_addr = new(struct sockaddr_in);

    sock_addr->sin_family      = AF_INET;
    sock_addr->sin_addr.s_addr = inet_addr(host);
    sock_addr->sin_port        = htons(port);

    connection = new(TCP_Connection);

    connection->address = sock_addr;
    connection->socket  = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (connection->socket == -1)
    {
        print_error("TCP connection error\n");
        goto error;
    }

    if(connect(connection->socket, sock_addr, sizeof(struct sockaddr_in)))
    {
        printf("errno %d\n", errno);
        print_error("TCP connection error\n");
        goto error;
    }

    return connection;

error:
    print_error("connection not found\n");

    return 0;
}


void destroy_TCP (TCP_Connection *connection)
{

}


void write_in_TCP (TCP_Connection *connection, Byte *data, int length_data)
{
	send(connection->socket, data, length_data, 0);
}


void read_from_TCP (TCP_Connection *connection, Byte *data, int length_data)
{
	recv(connection->socket, data, length_data, 0);
}