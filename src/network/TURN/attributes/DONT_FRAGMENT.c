void add_DONT_FRAGMENT_attribute(String *message)
{
	add_STUN_atribute_head(message, DONT_FRAGMENT_TURN_ATTRIBUTE, 0);
}