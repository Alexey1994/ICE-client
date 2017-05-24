void STUN_request(NetworkConnection  connection, String *message)
{
    write_in_network_connection(connection, message->begin, message->length);

#if ENABLE_STUN_DEBUG
    print_STUN_request(message);
#endif
}


void begin_STUN_request(String **message, unsigned short type)
{
	*message = create_STUN_head(type);
}


void end_STUN_request(NetworkConnection connection, String *message)
{
    set_STUN_content_length(message->begin, message->length - 20);
    STUN_request(connection, message);
    destroy_string(message);
}