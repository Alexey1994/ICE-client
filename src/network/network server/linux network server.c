typedef struct
{
    Server *server;
    int   (*run_on_request)(Network_Source source, Byte *arguments);
    Byte   *request_arguments;
}
Server_Update_arguments;


static void update_server(Server_Update_arguments *arguments)
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


static Server* create_server(
    Byte           *host,
    unsigned short  port,
    int             connection_type,
    int             connection_protocol,
    int           (*run_on_request)(Network_Source source, Byte *arguments),
    Byte           *request_arguments
)
{
    struct hostent     *host_data;
    struct sockaddr_in  sock_addr;

    Server                  *server    = new(Server);
    Server_Update_arguments *arguments = new(Server_Update_arguments);

    arguments->server            = server;
    arguments->run_on_request    = run_on_request;
    arguments->request_arguments = request_arguments;

    host_data = gethostbyname(host);

    if(!host_data)
        goto error;

    sock_addr.sin_port        = htons(port);
    sock_addr.sin_family      = AF_INET;
    sock_addr.sin_addr.s_addr = *((unsigned long*)host_data->h_addr);

    server->source = socket(AF_INET, connection_type, connection_protocol);

    if (server->source < 0)
        goto error;

    if(bind(server->source, &sock_addr, sizeof(struct sockaddr_in)) < 0)
        goto error;

    listen(server->source, SOMAXCONN);
    run_thread(update_server, arguments);

    return server;

error:
    return 0;
}


Server* create_UDP_server(
    Byte           *host,
    unsigned short  port,
    int           (*run_on_request)(Network_Source source, Byte *arguments),
    Byte           *request_arguments
)
{
    return create_server(host, port, SOCK_DGRAM, IPPROTO_UDP, run_on_request, request_arguments);
}


Server* create_TCP_server(
    Byte           *host,
    unsigned short  port,
    int           (*run_on_request)(Network_Source source, Byte *arguments),
    Byte           *request_arguments
)
{
    return create_server(host, port, SOCK_STREAM, IPPROTO_TCP, run_on_request, request_arguments);
}


void destroy_server(Server *server)
{

}
