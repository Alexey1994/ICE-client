#include <errno.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>


typedef struct
{
    Server *server;
    int   (*run_on_request)(Network_Source source, Byte *arguments);
    Byte   *request_arguments;
}
Server_Update_arguments;


static void update_UDP_server(Server_Update_arguments *arguments)
{
    Server *server                                                  = arguments->server;
    int   (*run_on_request)(Network_Source source, Byte *arguments) = arguments->run_on_request;
    Byte   *request_arguments                                       = arguments->request_arguments;
    free(arguments);

    for(;;)
    {
        Byte               buffer[1024];
        int                buffer_size;
        struct sockaddr_in client_address;
        int                client_address_size;

        client_address_size = sizeof(struct sockaddr_in);
        buffer_size = recvfrom(server->source, buffer, 1024, 0, &client_address, &client_address_size);

        if(buffer_size == -1)
        {
            /*if(WSAGetLastError() == WSAEWOULDBLOCK)
            {
                sleep_thread(1);
                continue;
            }*/

            //printf("error in listening server %d\n", WSAGetLastError());
            //break;
            continue;
        }

        if(!buffer_size)
            printf("skip packet\n");

        printf("packet readed\n");

        run_on_request(buffer, request_arguments);
    }
}


Server* create_UDP_server(
    Byte           *host,
    unsigned short  port,
    int           (*run_on_request)(Network_Source source, Byte *arguments),
    Byte           *request_arguments
)
{
    struct sockaddr_in      *server_address;
    Server                  *server;
    Server_Update_arguments *arguments;

    server = new(Server);
    server->address = new(struct sockaddr_in);
    server_address = server->address;
    memset(server_address, 0, sizeof(struct sockaddr_in));

    server_address->sin_family      = AF_INET;
    server_address->sin_addr.s_addr = inet_addr(host);
    server_address->sin_port        = htons(port);

    server->source = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if(server->source < 0)
    {
        print_error("socket error\n");
        goto error;
    }

    if(bind(server->source, server_address, sizeof(struct sockaddr_in)))
    {
        printf("bind error %d\n", errno);
        goto error;
    }

    //unsigned non_blocking = 1;
    //ioctlsocket(server->source, FIONBIO, &non_blocking);
    fcntl(server->source, F_SETFL, O_NONBLOCK);

    arguments = new(Server_Update_arguments);

    arguments->server            = server;
    arguments->run_on_request    = run_on_request;
    arguments->request_arguments = request_arguments;

    run_thread(update_UDP_server, arguments);

    return server;

error:
    printf("UDP server error\n");
    return 0;
}


static void update_TCP_server(Server_Update_arguments *arguments)
{
    Server *server                                                  = arguments->server;
    int   (*run_on_request)(Network_Source source, Byte *arguments) = arguments->run_on_request;
    Byte   *request_arguments                                       = arguments->request_arguments;
    free(arguments);

    for(;;)
    {
        Network_Source client = accept(server->source, 0, 0);

        if(client < 0)
            continue;

        run_on_request(client, request_arguments);
        sleep_thread(1);
    }
}


Server* create_TCP_server(
    Byte           *host,
    unsigned short  port,
    int           (*run_on_request)(Network_Source source, Byte *arguments),
    Byte           *request_arguments
)
{
    struct sockaddr_in       sock_addr;
    Server                  *server;
    Server_Update_arguments *arguments;

    memset(&sock_addr, 0, sizeof(struct sockaddr_in));

    sock_addr.sin_port        = htons(port);
    sock_addr.sin_family      = AF_INET;
    sock_addr.sin_addr.s_addr = inet_addr(host);

    server = new(Server);

    server->source = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(server->source == -1)
    {
        print_error("socket error\n");
        goto error;
    }

    if(bind(server->source, &sock_addr, sizeof(struct sockaddr_in)) == -1)
    {
        print_error("bind error\n");
        goto error;
    }

    if(listen(server->source, SOMAXCONN))
    {
        print_error("listen error\n");
        goto error;
    }

    arguments = new(Server_Update_arguments);

    arguments->server            = server;
    arguments->run_on_request    = run_on_request;
    arguments->request_arguments = request_arguments;

    run_thread(update_TCP_server, arguments);

    return server;

error:
    printf("TCP server error\n");
    return 0;
}


void destroy_server(Server *server)
{

}
