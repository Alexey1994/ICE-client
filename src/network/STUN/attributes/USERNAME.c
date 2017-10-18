void read_USERNAME_attribute(STUN_Attributes *attributes, Byte *attribute, int length)
{

}


void add_USERNAME(String *message, char *username)
{
	int username_length;
	int message_length;

	username_length = strlen(username);
	message_length = (username_length / 4 + 1) * 4;

	add_STUN_atribute_head(message, USERNAME, username_length);
	write_byte_array_to_string(message, username, username_length);

	//padding
	while(message_length - username_length)
	{
		push_in_string(message, 0x20);
		--message_length;
	}
}