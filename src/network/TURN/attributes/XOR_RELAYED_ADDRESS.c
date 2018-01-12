//
//       0                   1                   2                   3
//       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//      |0 0 0 0 0 0 0 0|    Family     |           Port                |
//      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//      |                                                               |
//      |                 Address (32 bits or 128 bits)                 |
//      |                                                               |
//      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+


void read_XOR_RELAYED_ADDRESS_attribute(TURN_Attributes *attributes, Byte *attribute, int length)
{
    N_32 ip;

    ip = *((N_32*)(attribute+4)) ^ STUN_COOKIE;
    attributes->XOR_RELAYED_ADDRESS.host = malloc(32);//*((N_32*)(attribute+4));
    attributes->XOR_RELAYED_ADDRESS.port = *((N_16*)(attribute + 2)) ^ (STUN_COOKIE % 65536);//*((N_16*)(attribute+2));

    convert_big_to_little_endian(&ip, 4);
    convert_big_to_little_endian(&attributes->XOR_RELAYED_ADDRESS.port, 2);

    snprintf(attributes->XOR_RELAYED_ADDRESS.host, 32, "%d.%d.%d.%d",
        (ip >> 24) % 256,
        (ip >> 16) % 256,
        (ip >> 8) % 256,
        ip % 256);

    printf("read %s\n", attributes->XOR_RELAYED_ADDRESS.host);
}
