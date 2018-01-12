void read_XOR_MAPPED_ADDRESS_attribute(STUN_Attributes *attributes, Byte *attribute, int length)
{
    attributes->XOR_MAPPED_ADDRESS.host = malloc(16);
    snprintf(attributes->XOR_MAPPED_ADDRESS.host, 16, "%d.%d.%d.%d", attribute[4], attribute[5], attribute[6], attribute[7]);
    attributes->XOR_MAPPED_ADDRESS.port = (attribute[2]<<8) + attribute[3];
}
