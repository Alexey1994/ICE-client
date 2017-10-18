void TURN_response_handler(Byte *data, Byte *end_response)
{
    if(!data)
    {
        *end_response = TIMEOUT_ERROR;
        return;
    }

    *end_response = NO_ERRORS;
}


String* TURN_response(UDP_Connection *connection)
{
    STUN_Head *head;

    String *message      = create_string(MAX_TURN_RESPONSE_LENGTH);
    Byte    end_response = 0;

    //async_read_from_network_connection(connection, 500, message->begin, MAX_TURN_RESPONSE_LENGTH, TURN_response_handler, &end_response);

    //while(!end_response);// waiting

    read_from_UDP(connection, message->begin, MAX_TURN_RESPONSE_LENGTH);

    if(end_response != NO_ERRORS)
        goto error;

    head = message->begin;
    message->length = head->content_length;
    convert_big_to_little_endian(&message->length, 2);
    message->length += 20;

#ifdef ENABLE_DEBUG
    print_TURN_response(message);
#endif

    return message;

error:
    destroy_string(message);
    return 0;
}


TURN_Attributes* get_response_TURN_attributes(UDP_Connection *connection)
{
    String *response_message = TURN_response(connection);

    if(!response_message)
        goto error;

    TURN_Attributes *attributes = create_TURN_attributes_from_message(response_message);
    destroy_string(response_message);

    return attributes;

error:
    return 0;
}
