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

    //char local_address[16];
    //get_IPv4_host_address(local_address);
    //printf("local address: %s\n", local_address);
/*
    sprintf(buf, "mapped address: %s:%d\n", mapped_host, mapped_port);
    print_log(buf);*/

    //TURN_TCP_request("192.168.56.1", 3478);

    //TURN("127.0.0.1", 3478);
    //bind_TURN_channel("127.0.0.1", 3478);
    //TURN("192.168.56.101", 3478);

    //return authenticate_on_STUN_server("192.168.56.101", 3478);

    //authenticate_on_STUN_server("192.168.56.101", 3478);
    //authenticate_on_STUN_server("192.168.56.1", 3478);

    get_STUN_mapped_address("127.0.0.1", 3478, mapped_host, &mapped_port);
    //printf("mapped to %s:%d\n", mapped_host, mapped_port);

    TCP_Connection *signal_server = create_TCP("localhost", 8080);

    char *data =
        "GET /reg1?address=123 HTTP/1.1\r\n"
        "Host: localhost:8080\r\n"
        "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:55.0) Gecko/20100101 Firefox/55.0\r\n"
        "Accept: text/html\r\n"
        "Connection: close\r\n\r\n";

    write_in_TCP(signal_server, data, strlen(data)+1);

    char request[512];

    read_from_TCP(signal_server, request, 512);

    printf(request);


    Server *sender = create_UDP_server("127.0.0.1", 80, sender_listener, 0);//create_UDP_server(mapped_host, mapped_port, listener, 0);
    //Server *receiver = create_UDP_server("0.0.0.0", 9, receiver_listener, 0);

    //UDP_Connection *server = create_UDP("192.168.56.1", 53241);
    //UDP_Connection *connection = create_UDP(mapped_host, mapped_port);
    UDP_Connection *connection = create_UDP("192.168.56.1", 60769);

    for(;;)
    {
        write_in_UDP(connection, "Hi", 3);
        sleep_thread(1000);

        //Byte tt[200];
        //read_from_UDP(server, tt, 2);
        //printf("%s", tt);
    }

    //closesocket();

    //get_STUN_mapped_address("192.168.56.101", 3478, mapped_host, &mapped_port);

    //authenticate_on_STUN_server("turn1.northeurope.cloudapp.azure.com", 3478);

    return 0;
}
