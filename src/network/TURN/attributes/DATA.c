void read_DATA_attribute(TURN_Attributes *attributes, Byte *attribute, int length)
{
	
}

void add_XOR_PEER_ADDRESS(String *message)
{
	add_STUN_atribute_head(message, XOR_PEER_ADDRESS_TURN_ATTRIBUTE, 0);
}