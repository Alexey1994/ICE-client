void add_LIFETIME(String *message, unsigned int lifetime)
{
	add_STUN_atribute_head(message, LIFETIME_TURN_ATTRIBUTE, 4);
	convert_big_to_little_endian(&lifetime, 4);
	write_byte_array_to_string(message, &lifetime, 4);
}