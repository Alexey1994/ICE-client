void read_NONCE_attribute(STUN_Attributes *attributes, Byte *attribute, int length)
{
    N_32 i;

    attributes->NONCE.length = length;
    attributes->NONCE.data = malloc(length);

    for(i=0; i<length; ++i)
        attributes->NONCE.data[i] = attribute[i];
}


void add_NONCE(String *message, char *NONCE, N_8 length)
{
	add_STUN_atribute_head(message, NONCE_STUN_ATTRIBUTE, length);
	write_byte_array_to_string(message, NONCE, length);

	while(length%4)
        push_in_string(message, 0);
}
