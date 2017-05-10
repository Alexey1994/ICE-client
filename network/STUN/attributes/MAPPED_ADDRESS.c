#include "MAPPED_ADDRESS.h"


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


void read_MAPPED_ADDRESS_attribute(STUN_Attributes *attributes, Byte *attribute, int length)
{
    attributes->MAPPRED_ADDRESS.host = malloc(16);
    snprintf(attributes->MAPPRED_ADDRESS.host, 16, "%d.%d.%d.%d", attribute[4], attribute[5], attribute[6], attribute[7]);
    attributes->MAPPRED_ADDRESS.port = (attribute[2]<<8) + attribute[3];
}
