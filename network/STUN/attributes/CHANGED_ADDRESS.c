#include "CHANGED_ADDRESS.h"


void read_CHANGED_ADDRESS_attribute(STUN_Attributes *attributes, Byte *attribute, int length)
{
	attributes->CHANGED_ADDRESS.host = malloc(16);
    snprintf(attributes->CHANGED_ADDRESS.host, 16, "%d.%d.%d.%d", attribute[4], attribute[5], attribute[6], attribute[7]);
    attributes->CHANGED_ADDRESS.port = (attribute[2]<<8) + attribute[3];
}


void add_attribute_head(Stream *attributes, unsigned short type, unsigned short length);


void add_CHANGED_ADDRESS_attribute(Stream *attributes, char ip_version, char *host, unsigned short port)
{
    add_attribute_head(attributes, CHANGED_ADDRESS, 8);

    if(ip_version == 4)
        put_stream_byte(attributes, 1);
    else if(ip_version == 6)
        put_stream_byte(attributes, 2);

    convert_big_to_little_endian(&port, 2);
    write_data_in_output_stream(attributes, &port, 2);
    write_text_in_output_stream(attributes, host);
}