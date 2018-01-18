void read_XOR_MAPPED_ADDRESS_attribute(STUN_Attributes *attributes, Byte *attribute, int length)
{
    N_32 ip;

    ip = *((N_32*)(attribute+4)) ^ STUN_COOKIE;
    attributes->XOR_MAPPED_ADDRESS.host = malloc(32);//*((N_32*)(attribute+4));
    attributes->XOR_MAPPED_ADDRESS.port = *((N_16*)(attribute + 2)) ^ (STUN_COOKIE % 65536);

    convert_big_to_little_endian(&ip, 4);
    convert_big_to_little_endian(&attributes->XOR_MAPPED_ADDRESS.port, 2);

    snprintf(attributes->XOR_MAPPED_ADDRESS.host, 32, "%d.%d.%d.%d",
        (ip >> 24) % 256,
        (ip >> 16) % 256,
        (ip >> 8) % 256,
        ip % 256);
}
