#define ENABLE_DEBUG

#include "main/main.h"
//#include "network/network info/network info.h"
#include "network/STUN/STUN.h"
//#include "network/TURN/TURN.h"
#include "network/network server/network server.h"
//#include "network/connection/UDP/UDP.h"

#include "network/connection/TCP/TCP.h"


int sender_listener(Byte *data)
{
    printf("listen client\n");
    //printf("listen client %s\n", data);
}


int receiver_listener(Byte *data)
{
    printf("listen client\n");
    //printf("listen client %s\n", data);
}


void read_HTTP_data(char *HTTP_data, char *buffer)
{
    for(; *HTTP_data; ++HTTP_data)
    {
        if(*HTTP_data == '\r' && HTTP_data[1] == '\n' && HTTP_data[2] == '\r' && HTTP_data[3] == '\n')
        {
            HTTP_data += 3;
            break;
        }
    }

    strcpy(buffer, HTTP_data);
}


void send_mapped_address(char *host, int port)
{
    TCP_Connection *signal_server = create_TCP("127.0.0.1", 8080);
    char            buffer[512];

    snprintf(buffer, 512,
        "GET /reg1?host=%s&port=%d HTTP/1.1\r\n"
        "Host: localhost:8080\r\n"
        "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:55.0) Gecko/20100101 Firefox/55.0\r\n"
        "Accept: text/html\r\n"
        "Connection: close\r\n\r\n",
        host,
        port
    );

    write_in_TCP(signal_server, buffer, strlen(buffer));
    memset(buffer, 0, 512);
    read_from_TCP(signal_server, buffer, 512);
}


void get_client_host(char *host)
{
    TCP_Connection *signal_server = create_TCP("127.0.0.1", 8080);
    char            buffer[512];
    int             i;

    snprintf(buffer, 512,
        "GET /host1 HTTP/1.1\r\n"
        "Host: localhost:8080\r\n"
        "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:55.0) Gecko/20100101 Firefox/55.0\r\n"
        "Accept: text/html\r\n"
        "Connection: close\r\n\r\n"
    );

    write_in_TCP(signal_server, buffer, strlen(buffer));
    memset(buffer, 0, 512);
    read_from_TCP(signal_server, buffer, 512);

    read_HTTP_data(buffer, host);
}


int get_client_port()
{
    TCP_Connection *signal_server = create_TCP("127.0.0.1", 8080);
    char            buffer[512];
    int             i;

    snprintf(buffer, 512,
        "GET /port1 HTTP/1.1\r\n"
        "Host: localhost:8080\r\n"
        "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:55.0) Gecko/20100101 Firefox/55.0\r\n"
        "Accept: text/html\r\n"
        "Connection: close\r\n\r\n"
    );

    write_in_TCP(signal_server, buffer, strlen(buffer));

    memset(buffer, 0, 512);
    read_from_TCP(signal_server, buffer, 512);
    read_HTTP_data(buffer, buffer);

    return atoi(buffer);
}


int main(int arguments_length, char *arguments[])
{
    char           buf[200];
    char           mapped_host[16];
    unsigned short mapped_port;

    initialize_STUN();
    //initialize_TURN();

/*
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


    get_STUN_mapped_address("127.0.0.1", 3478, mapped_host, &mapped_port);


    send_mapped_address(mapped_host, mapped_port);

    char client_host[512];
    int client_port;

    get_client_host(client_host);
    client_port = get_client_port();


    Server *sender = create_UDP_server("127.0.0.1", 80, sender_listener, 0);

    UDP_Connection *connection = create_UDP(client_host, client_port);
    printf("connect to client address: %s:%d\n", client_host, client_port);

    for(;;)
    {
        write_in_UDP(connection, "Hi", 3);
        sleep_thread(1000);

    }

    return 0;
}
