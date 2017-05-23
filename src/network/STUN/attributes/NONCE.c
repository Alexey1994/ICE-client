void read_NONCE_attribute(STUN_Attributes *attributes, Byte *attribute, int length)
{

}


void add_NONCE(String *message, char *cookie_128)
{
	char cookie[16];

	add_STUN_atribute_head(message, NONCE, 16);
	write_byte_array_to_string(message, cookie, 16);
}