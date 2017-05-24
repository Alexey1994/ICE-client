void TURN_request(NetworkConnection connection, String *message)
{
    write_in_network_connection(connection, message->begin, message->length);

#if ENABLE_STUN_DEBUG
    print_TURN_request(message);
#endif
}
