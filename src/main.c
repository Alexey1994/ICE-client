#define ENABLE_DEBUG

#include "main/main.h"
//#include "network/network info/network info.h"
#include "network/STUN/STUN.h"
#include "network/TURN/TURN.h"
#include "network/network server/network server.h"
//#include "network/connection/UDP/UDP.h"
#include "network/connection/TCP/TCP.h"

#define CLIENT_ID 2

#define STUN_HOST "192.168.56.101"
#define STUN_PORT 3478


#define TURN_HOST "192.168.56.101"
#define TURN_PORT 3478

#define SIGNAL_SERVER_HOST "127.0.0.1"
#define SIGNAL_SERVER_PORT 8080


int sender_listener(Byte *data)
{
    printf("message: %s\n", data);
    //printf("listen client\n");
    //printf("listen client %s\n", data);
}


void read_HTTP_data(char *HTTP_data, char *buffer)
{
    for(; *HTTP_data; ++HTTP_data)
    {
        if(*HTTP_data == '\r' && HTTP_data[1] == '\n' && HTTP_data[2] == '\r' && HTTP_data[3] == '\n')
        {
            HTTP_data += 4;
            break;
        }
    }

    strcpy(buffer, HTTP_data);
}


void send_mapped_address(char *host, int port)
{
    TCP_Connection *signal_server = create_TCP(SIGNAL_SERVER_HOST, SIGNAL_SERVER_PORT);
    char            buffer[512];

    snprintf(buffer, 512,
        "GET /reg%d?host=%s&port=%d HTTP/1.1\r\n"
        "Host: %s:%d\r\n"
        "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:55.0) Gecko/20100101 Firefox/55.0\r\n"
        "Accept: text/html\r\n"
        "Connection: close\r\n\r\n",
        CLIENT_ID,

        host,
        port,

        SIGNAL_SERVER_HOST,
        SIGNAL_SERVER_PORT
    );

    write_in_TCP(signal_server, buffer, strlen(buffer));
    memset(buffer, 0, 512);
    read_from_TCP(signal_server, buffer, 512);
}


void get_client_host(char *host)
{
    TCP_Connection *signal_server = create_TCP(SIGNAL_SERVER_HOST, SIGNAL_SERVER_PORT);
    char            buffer[512];
    int             i;

    snprintf(buffer, 512,
        "GET /host%d HTTP/1.1\r\n"
        "Host: %s:%d\r\n"
        "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:55.0) Gecko/20100101 Firefox/55.0\r\n"
        "Accept: text/html\r\n"
        "Connection: close\r\n\r\n",
        CLIENT_ID,
        SIGNAL_SERVER_HOST,
        SIGNAL_SERVER_PORT
    );

    write_in_TCP(signal_server, buffer, strlen(buffer));
    memset(buffer, 0, 512);
    read_from_TCP(signal_server, buffer, 512);

    read_HTTP_data(buffer, host);
}


int get_client_port()
{
    TCP_Connection *signal_server = create_TCP(SIGNAL_SERVER_HOST, SIGNAL_SERVER_PORT);
    char            buffer[512];
    int             i;

    snprintf(buffer, 512,
        "GET /port%d HTTP/1.1\r\n"
        "Host: %s:%d\r\n"
        "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:55.0) Gecko/20100101 Firefox/55.0\r\n"
        "Accept: text/html\r\n"
        "Connection: close\r\n\r\n",
        CLIENT_ID,
        SIGNAL_SERVER_HOST,
        SIGNAL_SERVER_PORT
    );

    write_in_TCP(signal_server, buffer, strlen(buffer));

    memset(buffer, 0, 512);
    read_from_TCP(signal_server, buffer, 512);
    read_HTTP_data(buffer, buffer);

    return atoi(buffer);
}


void test_STUN_connection()
{
    char           buffer[512];
    char           mapped_host[16];
    unsigned short mapped_port;
    char           client_host[512];
    int            client_port;

    get_STUN_mapped_address(STUN_HOST, STUN_PORT, mapped_host, &mapped_port);
    send_mapped_address(mapped_host, mapped_port);
    Server *sender = create_UDP_server(mapped_host, mapped_port, sender_listener, 0);
    printf("listen messages on address: %s:%d\n", mapped_host, mapped_port);

    sleep_thread(5000);
    get_client_host(client_host);
    client_port = get_client_port();

    UDP_Connection *connection = create_UDP(client_host, client_port);
    printf("connect to client address: %s:%d\n", client_host, client_port);

    for(;;)
    {
        snprintf(buffer, 512, "Hello from client %d", CLIENT_ID);

        write_in_UDP(connection, buffer, strlen(buffer));
        sleep_thread(1000);
    }
}


void test_STUN()
{
    char           mapped_host[16];
    unsigned short mapped_port;

    get_STUN_mapped_address(STUN_HOST, STUN_PORT, mapped_host, &mapped_port);
}


void test_TURN_connection()
{
    N_32 xor_relayed_host;
    N_16 xor_relayed_port;

    allocate_TURN(TURN_HOST, TURN_PORT, &xor_relayed_host, &xor_relayed_port);
    create_TURN_permission(TURN_HOST, TURN_PORT, xor_relayed_host, xor_relayed_port);
    //bind_TURN_channel(TURN_HOST, TURN_PORT);
    //send_TURN_data(TURN_HOST, TURN_PORT);
    //receive_TURN_data(TURN_HOST, TURN_PORT);
}


void print_test_TURN_vectors(STUN_Head *head)
{
    String     message_string;

    message_string.length = head->content_length;
    convert_big_to_little_endian(&message_string.length, 2);
    message_string.length += 20;
    message_string.begin = head;

    print_TURN_response(&message_string);
}


void get_test_vectors()
{
    unsigned char buffer[512];

    Server *sender = create_UDP_server("127.0.0.1", 8081, print_test_TURN_vectors, 0);

    for(;;);
}


int main(int arguments_length, char *arguments[])
{
    initialize_STUN();
    initialize_TURN();
    //get_test_vectors();
    //test_STUN();
    test_TURN_connection();
    //test_MD5_hash();

    /*
    char           mapped_host[16];
    unsigned short mapped_port;

    if(arguments_length < 2)
    {
        if (!get_STUN_mapped_address("192.168.56.101", 3478, mapped_host, &mapped_port))
            return 0;
    }
    else if(arguments_length == 2)
    {
        if (!get_STUN_mapped_address(arguments[1], 3478, mapped_host, &mapped_port))
            return 0;
    }
    else
    {
        if (!get_STUN_mapped_address(arguments[1], atoi(arguments[2]), mapped_host, &mapped_port))
            return 0;
    }*/

    return 0;
}
