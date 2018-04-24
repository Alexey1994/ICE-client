internal function Boolean initialize_UDP_Connection (UDP_Connection *connection, Address *destination, N_32 maximum_waiting_time)
{
    Boolean is_non_blocking_mode;
    N_32    connection_status;

    set_UDP_Connection_destination(connection, destination);
    connection->socket = socket(destination->family, SOCKET_DATAGRAMS, UDP_PROTOCOL);

    if(connection->socket == -1)
        goto error;

    connection->maximum_waiting_time = maximum_waiting_time;
    connection->current_time = 0;

    is_non_blocking_mode = 1;

    if(ioctl(connection->socket, FIONBIO, &is_non_blocking_mode))
    {
        close(connection->socket);
        goto error;
    }

    return 1;

error:
    return 0;
}


internal procedure deinitialize_UDP_Connection (UDP_Connection *connection)
{
    close(connection->socket);
}


internal function Boolean set_UDP_Connection_source (UDP_Connection *connection, Address *source)
{
    connection->source = source;

    if(bind(connection->socket, source, sizeof(Address)))
        goto error;

    return 1;

error:
    return 0;
}


internal procedure set_UDP_Connection_destination (UDP_Connection *connection, Address *destination)
{
    connection->destination = destination;
}


internal function Boolean read_from_UDP_Connection(UDP_Connection *connection, Byte *data, N_32 *length)
{
    Z_32 status;

    connection->current_time = 0;

    loop
    {
        status = recv(connection->socket, data, *length, 0);

        if(status >= 0)
            break;

        if(errno != EAGAIN)
            goto error;

        if(connection->current_time > connection->maximum_waiting_time)
            goto error;

        sleep_thread(1);
        ++connection->current_time;
    }

    *length = status;

    return 1;

error:
    return 0;
}


internal function Boolean write_in_UDP_Connection(UDP_Connection *connection, Byte *data, N_32 length)
{
    connection->current_time = 0;

    while(sendto(connection->socket, data, length, 0, connection->destination, sizeof(Address)) == -1)
    {
        if(errno != EAGAIN && errno != EWOULDBLOCK)
            goto error;

        if(connection->current_time > connection->maximum_waiting_time)
            goto error;

        sleep_thread(1);
        ++connection->current_time;
    }

    return 1;

error:
    return 0;
}
