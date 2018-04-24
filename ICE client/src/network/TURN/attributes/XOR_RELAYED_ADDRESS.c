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


private procedure read_XOR_RELAYED_ADDRESS_attribute (TURN_Client *client)
{
    N_32 i;

    memset(&client->parser.attributes.XOR_RELAYED_ADDRESS, 0, sizeof(Address));

    read_binary_N_8(&client->STUN_client.buffer_input);
    client->parser.attributes.XOR_RELAYED_ADDRESS.family = read_binary_N_8(&client->STUN_client.buffer_input);

    for(i=0; i<2; ++i)
        ((Byte*)&client->parser.attributes.XOR_RELAYED_ADDRESS.port)[i] = read_binary_N_8(&client->STUN_client.buffer_input) ^ STUN_COOKIE[i];

    if(client->parser.attributes.XOR_RELAYED_ADDRESS.family == 1)
    {
        client->parser.attributes.XOR_RELAYED_ADDRESS.family = IPv4;

        for(i=0; i<4; ++i)
            ((Byte*)&client->parser.attributes.XOR_RELAYED_ADDRESS.ip_v4)[i] = read_binary_N_8(&client->STUN_client.buffer_input) ^ STUN_COOKIE[i];
    }
    else if(client->parser.attributes.XOR_RELAYED_ADDRESS.family == 2)
    {
        client->parser.attributes.XOR_RELAYED_ADDRESS.family = IPv6;

        for(i=0; i<4; ++i)
            client->parser.attributes.XOR_RELAYED_ADDRESS.ip_v6[i] = read_byte(&client->STUN_client.buffer_input) ^ STUN_COOKIE[i];

        for(; i<16; ++i)
            client->parser.attributes.XOR_RELAYED_ADDRESS.ip_v6[i] = read_byte(&client->STUN_client.buffer_input) ^ (client->STUN_client.parser.head.transaction_id[i - 4]);
    }
}
