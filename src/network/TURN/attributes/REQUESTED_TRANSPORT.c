//      0                   1                   2                   3
//      0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//     |    Protocol   |                    RFFU                       |
//     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+


void add_REQUESTED_TRANSPORT(String *message, Byte transport_type)
{
	add_STUN_atribute_head(message, REQUESTED_TRANSPORT_TURN_ATTRIBUTE, 4);

	switch(transport_type)
	{
		case UDP_CONNECTION: push_in_string(message, 17); break;
		case TCP_CONNECTION: push_in_string(message, 17); break;

		default:
			push_in_string(message, 17);
	}

	push_in_string(message, 0);
	push_in_string(message, 0);
	push_in_string(message, 0);
}
