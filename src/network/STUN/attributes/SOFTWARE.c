void read_SOFTWARE_attribute(STUN_Attributes *attributes, Byte *attribute, int length)
{

}


void add_SOFTWARE(String *message, char *text)
{
	char buf[512];
	add_STUN_atribute_head(message, SOFTWARE, 508);

	snprintf(buf, 16, "%s", text);

	write_byte_array_to_string(message, buf, 508);
}