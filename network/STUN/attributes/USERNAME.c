void read_USERNAME_attribute(STUN_Attributes *attributes, Byte *attribute, int length)
{

}


void add_USERNAME_attribute_to_STUN_message(String *message, char *username)
{
	char buf[512];
	add_STUN_atribute_head(message, USERNAME, 508);

	snprintf(buf, 16, "%s", username);

	write_byte_array_to_string(message, buf, 508);
}