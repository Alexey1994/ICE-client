void read_XOR_PEER_ADDRESS_attribute(TURN_Attributes *attributes, Byte *attribute, int length)
{
    //attributes->XOR_PEER_ADDRESS.host = *((N_32*)(attribute+4));
    //attributes->XOR_PEER_ADDRESS.port = *((N_16*)(attribute+2));
/*
    N_32 ip;

    ip = *((N_32*)(attribute+4));
    attributes->XOR_PEER_ADDRESS.host = malloc(16);//*((N_32*)(attribute+4));
    attributes->XOR_PEER_ADDRESS.port = *((N_32*)(attribute + 2)) ^ (STUN_COOKIE % 65536);//*((N_16*)(attribute+2));

    convert_big_to_little_endian(&ip, 4);
    convert_big_to_little_endian(&attributes->XOR_PEER_ADDRESS.port, 2);

    snprintf(attributes->XOR_PEER_ADDRESS.host, 16, "%d.%d.%d.%d",
        (ip >> 24) % 256,
        (ip >> 16) % 256,
        (ip >> 8) % 256,
        ip % 256);*/
}


void add_XOR_PEER_ADDRESS(String *message, Byte *host, N_16 port)
{
    N_32 ip;
    N_8  number;

	add_STUN_atribute_head(message, XOR_PEER_ADDRESS_TURN_ATTRIBUTE, 8);

	push_in_string(message, 0);
    push_in_string(message, 1);

    convert_big_to_little_endian(&port, 2);
    //convert_big_to_little_endian(&host, 4);

    push_in_string(message, 0 ^ STUN_COOKIE);
    push_in_string(message, 0 ^ (STUN_COOKIE>>8));

    ip = 0;
    number = 0;

    while(*host != '.')
    {
        number = number*10 + (*host - '0');
        ++host;
    }

    ip += number;

    while(*host != '.')
    {
        number = number*10 + (*host - '0');
        ++host;
    }

    ++host;
    ip += number<<8;

    while(*host != '.')
    {
        number = number*10 + (*host - '0');
        ++host;
    }

    ++host;
    ip += number<<16;

    while(*host != '.')
    {
        number = number*10 + (*host - '0');
        ++host;
    }

    ip += number<<24;

    ip = ip ^ STUN_COOKIE;

    push_in_string(message, ip);
    push_in_string(message, ip>>8);
    push_in_string(message, ip>>16);
    push_in_string(message, ip>>24);
}
