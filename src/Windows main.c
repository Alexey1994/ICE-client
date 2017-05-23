int main(int arguments_length, char *arguments[])
{
    char           mapped_host[16];
    unsigned short mapped_port;

    initialize_STUN();
    initialize_TURN();
/*
    if(arguments_length < 2)
        if(!get_STUN_mapped_address("192.168.56.1", 3478, mapped_host, &mapped_port))
            return 0;
    else if(arguments_length == 2)
        if(!get_STUN_mapped_address(arguments[1], 3478, mapped_host, &mapped_port))
            return 0;
    else
        if(!get_STUN_mapped_address(arguments[1], atoi(arguments[2]), mapped_host, &mapped_port))
            return 0;

    char local_address[16];
    get_IPv4_host_address(local_address);
    printf("local address: %s\n", local_address);
    printf("mapped address: %s:%d\n", mapped_host, mapped_port);
*/
    //TURN_TCP_request("192.168.56.1", 3478);

    //TURN("192.168.56.101", 3478);

    authenticate_on_STUN_server("192.168.56.101", 3478);

    //authenticate_on_STUN_server("turn1.northeurope.cloudapp.azure.com", 3478);
/*
    if(!get_STUN_mapped_address("turn1.northeurope.cloudapp.azure.com", 3478, mapped_host, &mapped_port))
        return 0;

    char local_address[16];
    get_IPv4_host_address(local_address);
    printf("local address: %s\n", local_address);
    printf("mapped address: %s:%d\n", mapped_host, mapped_port);
*/

/*
    NetworkConnection connection = create_TCP_connection("turn1.northeurope.cloudapp.azure.com", 3478);

    write_in_network_connection(connection,
        "GET / HTTP/1.1\r\n"
        "Host: turn1.northeurope.cloudapp.azure.com\r\n"
        "User-Agent: Mozilla/5.0 (X11; U; Linux i686; ru; rv:1.9b5) Gecko/2008050509 Firefox/3.0b5\r\n"
        "Connection: close\r\n"
        "\r\n",

        strlen(
            "GET / HTTP/1.1\r\n"
            "Host: turn1.northeurope.cloudapp.azure.com\r\n"
            "User-Agent: Mozilla/5.0 (X11; U; Linux i686; ru; rv:1.9b5) Gecko/2008050509 Firefox/3.0b5\r\n"
            "Connection: close\r\n"
            "\r\n") + 1
    );

    printf("%c", get_TCP_byte(connection));
    printf("%c", get_TCP_byte(connection));
    printf("%c", get_TCP_byte(connection));
    printf("%c", get_TCP_byte(connection));
    printf("%c", get_TCP_byte(connection));
    printf("%c", get_TCP_byte(connection));
    printf("%c", get_TCP_byte(connection));
    printf("%c", get_TCP_byte(connection));
    printf("%c", get_TCP_byte(connection));*/

    return 0;
}
