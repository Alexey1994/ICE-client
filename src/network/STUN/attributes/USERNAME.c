#include "../../../cryptography/cryptography.h"


void read_USERNAME_attribute(STUN_Attributes *attributes, Byte *attribute, int length)
{

}


void add_USERNAME(String *message, char *username)
{
	int username_length;

	username_length = strlen(username);

	add_STUN_atribute_head(message, USERNAME_STUN_ATTRIBUTE, username_length);
	write_byte_array_to_string(message, username, username_length);

	while(username_length%4)
    {
        push_in_string(message, 0x00);
		++username_length;
    }
}
