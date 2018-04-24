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
/*
    if(ioctl(connection->socket, FIONBIO, &is_non_blocking_mode))
    {
        close(connection->socket);
        goto error;
    }
*/
    Address a;
    initialize_address(&a, "127.0.0.1", "8023", UDP_PROTOCOL);

    set_UDP_Connection_source(connection, &a);
    
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
/*
    loop
    {
        status = recv(connection->socket, data, *length, 0);
//printf("status %d errno %d\n", status, errno);

        if(status >= 0)
            break;

        if(errno != EAGAIN)
            goto error;

        if(connection->current_time > connection->maximum_waiting_time)
            goto error;

        sleep_thread(1);
        ++connection->current_time;
    }
*/
printf("recv\n");

    //status = recv(connection->socket, data, *length, 0);

Address address;
N_32 address_length = sizeof(Address);
status = recvfrom(connection->socket, data, *length, 0, &address, &address_length);

    if(status < 0)
        goto error;

    *length = status;
printf("receive %d bytes\n", status);
    return 1;

error:
printf("error\n");
    return 0;
}


internal function Boolean write_in_UDP_Connection(UDP_Connection *connection, Byte *data, N_32 length)
{
    N_32 status;

    connection->current_time = 0;

write_address(&debug_output, connection->destination);
printf("\n");
    status = sendto(connection->socket, data, length, 0, connection->destination, connection->destination->length);

    if(status == -1)
        goto error;
/*
    for(;;)
    {
        status = sendto(connection->socket, data, length, 0, connection->destination, connection->destination->length);

        if(status == -1)
            goto error;

        printf("data length %d status %d errno %d\n", length, status, errno);

        if(errno == ETIMEDOUT)
            break;

        if(connection->current_time > connection->maximum_waiting_time)
            goto error;

        sleep_thread(1);
        ++connection->current_time;
    }*/
/*
    while(sendto(connection->socket, data, length, 0, connection->destination, connection->destination->length) == -1)
    {
        if(errno != EAGAIN && errno != EWOULDBLOCK)
            goto error;

        if(connection->current_time > connection->maximum_waiting_time)
            goto error;

        sleep_thread(1);
        ++connection->current_time;
    }*/

    return 1;

error:
    return 0;
}
