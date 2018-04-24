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


private procedure read_MAPPED_ADDRESS_attribute(STUN_Client *client)
{
    N_32 i;

    read_binary_N_8(&client->buffer_input);
    client->parser.attributes.MAPPED_ADDRESS.family = read_binary_N_8(&client->buffer_input);
    read_byte_array(&client->buffer_input, &client->parser.attributes.MAPPED_ADDRESS.port, 2);

    if(client->parser.attributes.MAPPED_ADDRESS.family == 1)
    {
        client->parser.attributes.MAPPED_ADDRESS.family = IPv4;
        read_byte_array(&client->buffer_input, &client->parser.attributes.MAPPED_ADDRESS.ip_v4, 4);
    }
    else if(client->parser.attributes.MAPPED_ADDRESS.family == 2)
    {
        client->parser.attributes.MAPPED_ADDRESS.family = IPv6;

        for(i=0; i<16; ++i)
            client->parser.attributes.MAPPED_ADDRESS.ip_v6[i] = read_byte(&client->buffer_input);
    }
}
