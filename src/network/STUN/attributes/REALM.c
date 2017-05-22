void read_REALM_attribute(STUN_Attributes *attributes, Byte *attribute, int length)
{

}


void add_REALM_attribute(String *message)
{
	char buf[16];
	add_STUN_atribute_head(message, REALM, 16);

	write_byte_array_to_string(message, buf, 16);
}