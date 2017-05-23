void begin_STUN_message(String **message, unsigned short type)
{
	*message = create_STUN_head(type);
    //set_STUN_content_length(message->begin, message->length - 20);
}


void end_STUN_message(String *message)
{
    set_STUN_content_length(message->begin, message->length - 20);
}


STUN_Attributes* create_STUN_attributes_from_message(String *message)
{
    STUN_Attributes *attributes  = new(STUN_Attributes);

    attributes->MAPPRED_ADDRESS.host = 0;
    attributes->CHANGED_ADDRESS.host = 0;

    read_STUN_attributes(attributes, message);

    return attributes;
}