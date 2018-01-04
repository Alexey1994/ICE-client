void read_DATA_attribute(TURN_Attributes *attributes, Byte *attribute, int length)
{

}


void add_DATA(String *message)
{
    add_STUN_atribute_head(message, DATA_TURN_ATTRIBUTE, 4);
    push_in_string(message, 0);
    push_in_string(message, 0);
    push_in_string(message, 0);
    push_in_string(message, 0);
}