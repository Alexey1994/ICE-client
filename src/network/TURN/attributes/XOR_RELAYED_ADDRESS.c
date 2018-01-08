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
    attributes->XOR_RELAYED_ADDRESS.host = *((N_32*)(attribute+4));
    attributes->XOR_RELAYED_ADDRESS.port = *((N_16*)(attribute+2));

    convert_big_to_little_endian(&attributes->XOR_RELAYED_ADDRESS.host, 4);
    convert_big_to_little_endian(&attributes->XOR_RELAYED_ADDRESS.port, 2);
}
