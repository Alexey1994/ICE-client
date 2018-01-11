void read_REALM_attribute(STUN_Attributes *attributes, Byte *attribute, int length)
{
    N_32 i;

    attributes->REALM = malloc(length + 1);

    for(i=0; i<length; ++i)
        attributes->REALM[i] = attribute[i];

    attributes->REALM[i] = '\0';
}


void add_REALM(String *message, Character *realm)
{
    N_32 realm_length;

    realm_length = strlen(realm);
	add_STUN_atribute_head(message, REALM_STUN_ATTRIBUTE, realm_length);

	write_byte_array_to_string(message, realm, realm_length);

	while(realm_length % 4)
        push_in_string(message, 0);
}
