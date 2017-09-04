static void STUN_response_handler(Byte *data, Byte *end_response)
{
    if(!data)
    {
        *end_response = TIMEOUT_ERROR;
        return;
    }

    *end_response = NO_ERRORS;
}


String* STUN_response(UDP_Connection *connection)
{
    STUN_Head *head;

    String *message      = create_string(MAX_STUN_RESPONSE_LENGTH);
    Byte    end_response = 0;

    //async_read_from_network_connection(connection, 500, message->begin, MAX_STUN_RESPONSE_LENGTH, STUN_response_handler, &end_response);

    //while(!end_response);// waiting

    //if(end_response != NO_ERRORS)
    //    goto error;

    read_from_UDP(connection, message->begin, MAX_STUN_RESPONSE_LENGTH);

    head = message->begin;
    message->length = head->content_length;
    convert_big_to_little_endian(&message->length, 2);
    message->length += 20;

#ifdef ENABLE_DEBUG
    print_STUN_response(message);
#endif

    return message;

error:
    destroy_string(message);
    return 0;
}


STUN_Attributes* get_response_STUN_attributes(UDP_Connection *connection)
{
    String *response_message = STUN_response(connection);

    if(!response_message)
        goto error;

    STUN_Attributes *attributes = create_STUN_attributes_from_message(response_message);
    destroy_string(response_message);

    return attributes;

error:
    return 0;
}
