static void(*read_attribute_TURN_handlers[65536])(TURN_Attributes *attributes, Byte *attribute, int attribute_length) = {};


Boolean read_TURN_attribute(TURN_Attribute *attribute, TURN_Attributes *attributes)
{/*
    void(*TURN_attribute_handler) (TURN_Attributes *attributes, Byte *attribute, int attribute_length);
    TURN_attribute_handler = read_attribute_TURN_handlers[ attribute->type ];

    if(!TURN_attribute_handler)
    {
        void(*STUN_attribute_handler) (STUN_Attributes *attributes, Byte *attribute, int attribute_length);
        STUN_attribute_handler = read_attribute_handlers[ attribute->type ];

        if(!STUN_attribute_handler)
            goto error;

        STUN_attribute_handler(attributes, (Byte*)attribute + 4, attribute->length);

        goto Ok;
    }

    TURN_attribute_handler(attributes, (Byte*)attribute + 4, attribute->length);

Ok:
    return 1;

error:
    return 0;*/
}


Boolean read_TURN_attributes(TURN_Attributes *attributes, String *message)
{/*
    int             length     = 20;
    STUN_Attribute *attribute  = message->begin + 20;

    while(length < message->length)
    {
        convert_big_to_little_endian(&attribute->type, 2);
        convert_big_to_little_endian(&attribute->length, 2);
        
        if(!read_TURN_attribute(attribute, attributes))
            goto error;

        length += 4 + attribute->length;
        attribute = (Byte*)attribute + 4 + attribute->length;
    }

    return 1;

error:
    return 0;*/
}


TURN_Attributes* create_TURN_attributes_from_message(String *message)
{
    TURN_Attributes *attributes  = new(TURN_Attributes);

    attributes->STUN_attributes = create_STUN_attributes();

    read_TURN_attributes(attributes, message);

    return attributes;
}


void destroy_TURN_attributes(TURN_Attributes *attributes)
{
    free(attributes);
}