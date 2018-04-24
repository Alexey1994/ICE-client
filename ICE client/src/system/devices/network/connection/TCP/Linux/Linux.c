internal function Boolean initialize_TCP_Connection (TCP_Connection *connection, Address *destination, N_32 maximum_waiting_time)
{
    Boolean is_non_blocking_mode;
    N_32    connection_status;

    connection->socket = socket(destination->family, SOCKET_STREAM, TCP_PROTOCOL);

    if(connection->socket == -1)
        goto error;

    is_non_blocking_mode = 1;
    ioctl(connection->socket, FIONBIO, &is_non_blocking_mode);

    connection->maximum_waiting_time = maximum_waiting_time;
    connection->current_time = 0;

    loop
    {
        connection_status = connect(connection->socket, destination, sizeof(Address));

        if(!connection_status)
            break;

        if(errno != EINPROGRESS && errno != EALREADY)
        {
            close(connection->socket);
            goto error;
        }

        if(connection->maximum_waiting_time && connection->current_time > connection->maximum_waiting_time)
        {
            close(connection->socket);
            goto error;
        }

        sleep_thread(1);
        ++connection->current_time;
    }

    return 1;

error:
    return 0;
}


internal procedure deinitialize_TCP_Connection (TCP_Connection *connection)
{
    close(connection->socket);
}


internal function Boolean read_from_TCP_Connection(TCP_Connection *connection, Byte *data, N_32 length)
{
    connection->current_time = 0;

    while(recv(connection->socket, data, length, 0) == -1)
    {
        if(errno != EAGAIN)
            goto error;

        if(connection->maximum_waiting_time && connection->current_time > connection->maximum_waiting_time)
            goto error;

        sleep_thread(1);
        ++connection->current_time;
    }

    return 1;

error:
    return 0;
}


internal function Boolean write_in_TCP_Connection(TCP_Connection *connection, Byte *data, N_32 length)
{
    connection->current_time = 0;

    while(send(connection->socket, data, length, 0) == -1)
    {
        if(errno != EAGAIN && errno != EWOULDBLOCK)
            goto error;

        if(connection->maximum_waiting_time && connection->current_time > connection->maximum_waiting_time)
            goto error;

        sleep_thread(1);
        ++connection->current_time;
    }

    return 1;

error:
    return 0;
}
