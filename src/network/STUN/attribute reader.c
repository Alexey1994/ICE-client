static void(*read_attribute_handlers[65536])(STUN_Attributes *attributes, Byte *attribute, int attribute_length) = {};


Boolean read_STUN_attribute(STUN_Attribute *attribute, STUN_Attributes *attributes)
{
    void(*attribute_handler)(STUN_Attributes *attributes, Byte *attribute, int attribute_length);

    attribute_handler = read_attribute_handlers[ attribute->type ];

    if(!attribute_handler)
        goto error;

    attribute_handler(attributes, (Byte*)attribute + 4, attribute->length);

    return 1;

error:
    return 0;
}


Boolean read_STUN_attributes(STUN_Attributes *attributes, String *message)
{
    int             length     = 20;
    STUN_Attribute *attribute  = message->begin + 20;

    while(length < message->length)
    {
        convert_big_to_little_endian(&attribute->type, 2);
        convert_big_to_little_endian(&attribute->length, 2);
        
        if(!read_STUN_attribute(attribute, attributes))
            goto error;
        
        length += 4 + attribute->length;
        attribute = (Byte*)attribute + 4 + attribute->length;
    }

    return 1;

error:
    return 0;
}


STUN_Attributes* create_STUN_attributes_from_message(String *message)
{
    STUN_Attributes *attributes  = new(STUN_Attributes);

    attributes->MAPPRED_ADDRESS.host = 0;
    attributes->CHANGED_ADDRESS.host = 0;

    read_STUN_attributes(attributes, message);

    return attributes;
}


STUN_Attributes* create_STUN_attributes()
{
    STUN_Attributes *attributes  = new(STUN_Attributes);

    attributes->MAPPRED_ADDRESS.host = 0;
    attributes->CHANGED_ADDRESS.host = 0;

    return attributes;
}


void destroy_STUN_attributes(STUN_Attributes *attributes)
{
    free(attributes->MAPPRED_ADDRESS.host);
    free(attributes);
}


STUN_Attributes* get_STUN_attributes()
{

}