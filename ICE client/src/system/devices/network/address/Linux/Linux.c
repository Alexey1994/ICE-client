internal function Boolean initialize_address (Address *address, Byte *host, Byte *port, N_32 protocol)
{
    Address_Info *address_info;
    Address_Info  hints;

    memset(&hints, 0, sizeof(Address_Info));

    if(protocol == UDP_PROTOCOL)
    {
        hints.family      = ANY_INTERNET_PROTOCOL;
        hints.socket_type = SOCKET_DATAGRAMS;
        hints.protocol    = UDP_PROTOCOL;
    }
    else if(protocol == TCP_PROTOCOL)
    {
        hints.family      = ANY_INTERNET_PROTOCOL;
        hints.socket_type = SOCKET_STREAM;
        hints.protocol    = TCP_PROTOCOL;
    }
    else
        goto error;

    if(getaddrinfo(host, port, &hints, &address_info))
        goto error;

    memset(address, 0, sizeof(Address));
    memcpy(address, address_info->address, address_info->address_length);

    freeaddrinfo(address_info);

    return 1;

error:
    return 0;
}