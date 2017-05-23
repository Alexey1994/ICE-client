static void STUN_response_handler(Byte *data, Byte *end_response)
{
    if(!data)
    {
        *end_response = TIMEOUT_ERROR;
        return;
    }

    *end_response = 1;
}


String* STUN_response(NetworkConnection connection)
{
    STUN_Head *head;
    Byte       end_response;

    String *message = create_string(200);

    async_read_from_network_connection(connection, 500, message->begin, 200, STUN_response_handler, &end_response);

    while(!end_response);// waiting

    if(end_response == TIMEOUT_ERROR)
        goto error;

    head = message->begin;
    convert_big_to_little_endian(&head->content_length, 2);
    message->length = 20 + head->content_length;

#if ENABLE_STUN_DEBUG
    print_STUN_response(message);
#endif

    return message;

error:
    destroy_string(message);
    return 0;
}