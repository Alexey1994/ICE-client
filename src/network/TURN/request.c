void TURN_request(NetworkConnection connection, String *message)
{
    write_in_network_connection(connection, message->begin, message->length);

#if ENABLE_STUN_DEBUG
    print_TURN_request(message);
#endif
}


void begin_TURN_request(String **message, unsigned short type)
{
	*message = create_STUN_head(type);
}


void end_TURN_request(NetworkConnection connection, String *message)
{
    set_STUN_content_length(message->begin, message->length - 20);
    TURN_request(connection, message);
    destroy_string(message);
}