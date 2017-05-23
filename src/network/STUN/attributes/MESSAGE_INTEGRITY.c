void read_MESSAGE_INTEGRITY_attribute(STUN_Attributes *attributes, Byte *attribute, int length)
{

}


void add_MESSAGE_INTEGRITY_attribute(String *message)
{
	char SHA1_hash[20];

	add_STUN_atribute_head(message, MESSAGE_INTEGRITY, 20);
	write_byte_array_to_string(message, SHA1_hash, 20);
}