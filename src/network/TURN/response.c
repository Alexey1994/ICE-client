void TURN_response_handler(Byte *data, Byte *end_response)
{
    if(!data)
    {
        *end_response = TIMEOUT_ERROR;
        return;
    }

    *end_response = 1;
}


String* TURN_response(NetworkConnection connection)
{
    STUN_Head *head;
    Byte       end_response;

    String *message = create_string(200);

    async_read_from_network_connection(connection, 500, message->begin, 200, TURN_response_handler, &end_response);

    while(!end_response);// waiting

    if(end_response == TIMEOUT_ERROR)
    {
        print_log("connection error\n");
        goto error;
    }

    head = message->begin;
    convert_big_to_little_endian(&head->content_length, 2);
    message->length = 20 + head->content_length;

    return message;

error:
    destroy_string(message);
    return 0;
}


TURN_Attributes* get_response_TURN_attributes(NetworkConnection connection)
{
    String *response_message = TURN_response(connection);

    if(!response_message)
        goto error;

    return create_TURN_attributes_from_message(response_message);

error:
    return 0;
}