#include "../../../stream/stream.h"


void read_CHANGED_ADDRESS_attribute(STUN_Attributes *attributes, Byte *attribute, int length)
{
	attributes->CHANGED_ADDRESS.host = malloc(16);
    snprintf(attributes->CHANGED_ADDRESS.host, 16, "%d.%d.%d.%d", attribute[4], attribute[5], attribute[6], attribute[7]);
    attributes->CHANGED_ADDRESS.port = (attribute[2]<<8) + attribute[3];
}


void add_CHANGED_ADDRESS(Stream *attributes, char ip_version, char *host, unsigned short port)
{
}
