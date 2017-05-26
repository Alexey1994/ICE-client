void read_USERNAME_attribute(STUN_Attributes *attributes, Byte *attribute, int length)
{

}


void add_USERNAME(String *message, char *username)
{
	char buf[20] = {};
	add_STUN_atribute_head(message, USERNAME, 20);

	snprintf(buf, 20, "%s", username);

	write_byte_array_to_string(message, buf, 20);
}