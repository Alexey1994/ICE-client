internal function Boolean initialize_TCP_Connection (TCP_Connection *connection, Address *destination, N_32 maximum_waiting_time)
{
    Boolean is_non_blocking_mode;
    N_32    connection_status;
    N_32    error_code;

    if(!initialize_Windows_sockets())
        goto error;

    connection->socket = socket(destination->family, SOCKET_STREAM, TCP_PROTOCOL);

    if(connection->socket == -1)
        goto error;

    is_non_blocking_mode = 1;
    ioctlsocket(connection->socket, NON_BLOCKING_MODE, &is_non_blocking_mode);

    connection->maximum_waiting_time = maximum_waiting_time;
    connection->current_time = 0;

    loop
    {
        connection_status = connect(connection->socket, destination, sizeof(Address));

        if(!connection_status)
            break;

        error_code = WSAGetLastError();

        if(error_code != RESOURCE_BLOCKED && error_code != SOCKET_ALREADY)
        {
            if(error_code == SOCKET_CONNECTED)
                break;

            closesocket(connection->socket);
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
    closesocket(connection->socket);
}


internal function Boolean read_from_TCP_Connection (TCP_Connection *connection, Byte *data, N_32 length)
{
    connection->current_time = 0;

    while(recv(connection->socket, data, length, 0) == -1)
    {
        if(WSAGetLastError() != RESOURCE_BLOCKED)
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
        if(WSAGetLastError() != RESOURCE_BLOCKED)
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
