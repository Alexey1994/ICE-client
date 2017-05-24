static void(*read_attribute_handlers[65536])(STUN_Attributes *attributes, Byte *attribute, int attribute_length) = {};


Boolean read_STUN_attribute(STUN_Attribute *attribute, STUN_Attributes *attributes)
{
    void(*attribute_handler)(STUN_Attributes *attributes, Byte *attribute, int attribute_length);

    unsigned short  attribute_length = attribute->length;
    unsigned short  attribute_type   = attribute->type;

    convert_big_to_little_endian(&attribute_type, 2);
    convert_big_to_little_endian(&attribute_length, 2);

    attribute_handler = read_attribute_handlers[ attribute_type ];

    if(!attribute_handler)
        goto error;

    attribute_handler(attributes, (Byte*)attribute + 4, attribute_length);

    return 1;

error:
    return 0;
}


Boolean read_STUN_attributes(STUN_Attributes *attributes, String *message)
{
    STUN_Attribute *attribute;
    unsigned short  attribute_length;

    int length = 20;

    while(length < message->length)
    {
        attribute = message->begin + length;

        if(!read_STUN_attribute(attribute, attributes))
            goto error;

        attribute_length = attribute->length;
        convert_big_to_little_endian(&attribute_length, 2);
        
        length += 4 + attribute_length;
    }

    return 1;

error:
    return 0;
}


STUN_Attributes* create_STUN_attributes_from_message(String *message)
{
    STUN_Attributes *attributes  = new(STUN_Attributes);

    attributes->MAPPED_ADDRESS.host = 0;
    attributes->CHANGED_ADDRESS.host = 0;

    read_STUN_attributes(attributes, message);

    return attributes;
}


STUN_Attributes* create_STUN_attributes()
{
    STUN_Attributes *attributes  = new(STUN_Attributes);

    attributes->MAPPED_ADDRESS.host = 0;
    attributes->CHANGED_ADDRESS.host = 0;

    return attributes;
}


void destroy_STUN_attributes(STUN_Attributes *attributes)
{
    free(attributes->MAPPED_ADDRESS.host);
    free(attributes);
}


STUN_Attributes* get_STUN_attributes()
{

}
