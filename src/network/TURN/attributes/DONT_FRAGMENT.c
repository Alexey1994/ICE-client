void add_DONT_FRAGMENT(String *message)
{
	add_STUN_atribute_head(message, DONT_FRAGMENT_TURN_ATTRIBUTE, 0);
}