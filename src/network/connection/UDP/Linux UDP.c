#include <errno.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>


UDP_Connection* create_UDP (Byte *host, int port)
{
    struct sockaddr_in *sock_addr;
    UDP_Connection     *connection;

    sock_addr = new(struct sockaddr_in);

    sock_addr->sin_family      = AF_INET;
    sock_addr->sin_addr.s_addr = inet_addr(host);
    sock_addr->sin_port        = htons(port);

    connection = new(UDP_Connection);

    connection->address = sock_addr;
    connection->socket  = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (connection->socket == -1)
    {
        print_error("UDP connection error\n");
        goto error;
    }

    //unsigned non_blocking = 1;
    //ioctlsocket(connection->socket, FIONBIO, &non_blocking);

    //while(connect(socket_connection, &sock_addr, sizeof(sock_addr)) == EINPROGRESS);

    //while(!connect(socket_connection, &sock_addr, sizeof(sock_addr)))
    //connect(socket_connection, &sock_addr, sizeof(sock_addr));

    return connection;

error:
    print_error("connection not found\n");

    return 0;
}


void destroy_UDP (UDP_Connection *connection)
{

}


void write_in_UDP (UDP_Connection *connection, Byte *data, int length_data)
{
    printf("write %d bytes\n", length_data);

    if(sendto(connection->socket, data, length_data, 0, connection->address, sizeof(struct sockaddr_in)) == -1)
        ;//print_error("error in write to network source\n");
}


void read_from_UDP (UDP_Connection *connection, Byte *data, int length_data)
{
    struct sockaddr_in client_address;
    int                client_address_size;
    printf("read %d bytes\n", length_data);
    int bytes_readed = recvfrom(connection->socket, data, length_data, 0, &client_address, &client_address_size);
}
