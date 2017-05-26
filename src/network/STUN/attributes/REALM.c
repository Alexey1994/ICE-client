void read_REALM_attribute(STUN_Attributes *attributes, Byte *attribute, int length)
{

}


void add_REALM(String *message)
{
	char buf[12] = {};
	add_STUN_atribute_head(message, REALM, 12);

	write_byte_array_to_string(message, buf, 12);
}