#include "debug.h"
#include "../../logger/logger.h"


static void (*print_TURN_attribute_handlers[65536])(Byte *attribute, int attribute_length);


void initialize_TURN_debug()
{
    int i;

    for(i=0; i<65536; ++i)
        print_TURN_attribute_handlers[i] = 0;

    print_TURN_attribute_handlers[DATA_TURN_ATTRIBUTE] = print_DATA_attribute;
}


void print_DATA_attribute(Byte *attribute, int length)
{
    char logbuf[200];
    snprintf(logbuf, 200, "\tRESPONSE ADDRESS, %d bytes\n", length);
    print_log(logbuf);
}


void print_TURN_attribute(TURN_Attribute *attribute)
{
    void (*attribute_handler)(Byte *attribute, int attribute_length);

    attribute_handler = print_TURN_attribute_handlers[ attribute->type ];

    if(!attribute_handler)
    {
        char logbuf[200];
        snprintf(logbuf, 200, "\tunknow attribute 0x%02x, %d bytes\n\n", attribute->type, attribute->length);
        print_log(logbuf);
        return;
    }

    attribute_handler((Byte*)attribute + 4, attribute->length);
    print_log("\n");
}


void print_TURN_attributes(String *message)
{
    int             length     = 20;
    Byte           *attributes = message->begin + 20;
    TURN_Attribute *attribute  = attributes;

    print_log("\n\tAttributes:\n\n");

    while(length < message->length)
    {
        print_TURN_attribute(attribute);
        length += 4 + attribute->length;
        attribute = (Byte*)attribute + 4 + attribute->length;
    }

    print_STUN_attributes(message);
}


void print_TURN_head(TURN_Header *header)
{
    print_STUN_head(header);
}


void print_TURN_request(String *message)
{
    print_log("TURN Request\n\n");
    print_TURN_head(message->begin);
    print_log("\n");
}


void print_TURN_response(String *message)
{
    print_log("TURN Response\n\n");
    print_TURN_head(message->begin);
    print_TURN_attributes(message);
    print_log("\n");
}