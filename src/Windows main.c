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

    TURN("127.0.0.1", 3478);

    //authenticate_on_STUN_server("192.168.56.101", 3478);
    //authenticate_on_STUN_server("192.168.56.101", 3478);
    //get_STUN_mapped_address("127.0.0.1", 3478, mapped_host, &mapped_port);
    //get_STUN_mapped_address("192.168.56.101", 3478, mapped_host, &mapped_port);

    //authenticate_on_STUN_server("turn1.northeurope.cloudapp.azure.com", 3478);

    return 0;
}
