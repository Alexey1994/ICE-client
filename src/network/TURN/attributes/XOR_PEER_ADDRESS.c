void read_XOR_PEER_ADDRESS_attribute(TURN_Attributes *attributes, Byte *attribute, int length)
{
    //attributes->XOR_PEER_ADDRESS.host = *((N_32*)(attribute+4));
    //attributes->XOR_PEER_ADDRESS.port = *((N_16*)(attribute+2));
}


void add_XOR_PEER_ADDRESS(String *message, N_32 host, N_16 port)
{
	add_STUN_atribute_head(message, XOR_PEER_ADDRESS_TURN_ATTRIBUTE, 8);

	push_in_string(message, 0);
    push_in_string(message, 1);

    convert_big_to_little_endian(&port, 2);
    convert_big_to_little_endian(&host, 4);

    //push_in_string(message, port);
    //push_in_string(message, port>>8);
    push_in_string(message, 0 ^ STUN_COOKIE);
    push_in_string(message, 0 ^ (STUN_COOKIE>>8));

    push_in_string(message, host);
    push_in_string(message, host>>8);
    push_in_string(message, host>>16);
    push_in_string(message, host>>24);
}
