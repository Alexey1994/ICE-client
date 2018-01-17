#include <stdio.h>
#include <errno.h>
#include <WinSock2.h>
#include <WS2tcpip.h>


static WSADATA *wsa_data = 0;


private procedure receive_UDP_data(UDP_Connection *connection)
{
    Z_32 bytes_readed;

    while(connection->data_receiver_ID)
    {
        //bytes_readed = recvfrom(connection->socket, connection->buffer.data, connection->buffer.reserve, 0, 0, 0);
        int size_connection_address = sizeof(struct sockaddr_in);
        //bytes_readed = recvfrom(connection->receive_socket, connection->buffer.data, connection->buffer.reserve, 0, connection->address, &size_connection_address);
        bytes_readed = recv(connection->socket, connection->buffer.data, connection->buffer.reserve, 0);//recvfrom(connection->socket, connection->buffer.data, connection->buffer.reserve, 0, connection->address, &size_connection_address);
        printf("read %d\n", bytes_readed);

        if(bytes_readed>0)
            connection->buffer.end_index = bytes_readed;
    }

    //closesocket(connection->receive_socket);
    free(connection);
}


UDP_Connection* create_UDP (Byte *host, int port)
{
    struct sockaddr_in *sock_addr;
    UDP_Connection     *connection;
    struct hostent     *host_address;
    int                 i;

    if(!wsa_data)
    {
        wsa_data=new(WSADATA);
        WSAStartup(MAKEWORD(2, 2), wsa_data);
    }

    sock_addr = new(struct sockaddr_in);

    host_address = gethostbyname(host);

    if(!host_address)
    {
        print_error("UDP host ");
        print_error(host);
        print_error(" not found\n");
        goto error;
    }

    sock_addr->sin_family      = host_address->h_addrtype;
    sock_addr->sin_port        = htons(port);
    sock_addr->sin_addr.s_addr = *((N_32*)host_address->h_addr_list[0]);

    printf("%s %d.%d.%d.%d\n", host,
           sock_addr->sin_addr.s_addr%256,
           (sock_addr->sin_addr.s_addr>>8)%256,
           (sock_addr->sin_addr.s_addr>>16)%256,
           (sock_addr->sin_addr.s_addr>>24)%256);

    connection = new(UDP_Connection);

    connection->address = sock_addr;
    connection->socket  = socket(host_address->h_addrtype, SOCK_DGRAM, IPPROTO_UDP);

    if (connection->socket == -1)
    {
        print_error("UDP connection error\n");
        goto error;
    }

    //unsigned non_blocking = 1;
    //ioctlsocket(connection->socket, FIONBIO, &non_blocking);

    initialize_buffer(&connection->buffer, 4096);
    //connection->data_receiver_ID = run_thread(&receive_UDP_data, connection);

    connection->data_receiver_ID = 1;
    run_thread(&receive_UDP_data, connection);

    return connection;

error:
    free(sock_addr);
    print_error("connection not found\n");

    return 0;
}


void destroy_UDP (UDP_Connection *connection)
{
    connection->data_receiver_ID = 0;
}


void write_in_UDP (UDP_Connection *connection, Byte *data, int length_data)
{
    int t = sendto(connection->socket, data, length_data, 0, connection->address, sizeof(struct sockaddr_in));
    //int t = sendto(connection->socket, data, length_data, 0, 0, 0);
    printf("write %d\n", t);
}


void read_from_UDP (UDP_Connection *connection, Byte *data, int length_data)
{
    //struct sockaddr address;
    //N_32            address_length;

    //address_length = sizeof(address);
    //int t = recvfrom(connection->socket, data, length_data, 0, &address, &address_length);

    //for(;;)
    /*{
        int t = recvfrom(connection->socket, data, length_data, 0, 0, 0);
        printf("read %d\n", t);
    }*/

    N_32 i;

    while(!buffer_length(&connection->buffer))
        sleep_thread(1);

    for(i=0; i<length_data && i<buffer_length(&connection->buffer); ++i)
        data[i] = connection->buffer.data[i];
}
