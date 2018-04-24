private procedure read_XOR_PEER_ADDRESS_attribute (TURN_Client *client)
{
    N_32 i;

    read_binary_N_8(&client->STUN_client.buffer_input);
    client->parser.attributes.XOR_PEER_ADDRESS.family = read_binary_N_8(&client->STUN_client.buffer_input);
    read_byte_array(&client->STUN_client.buffer_input, &client->parser.attributes.XOR_PEER_ADDRESS.port, 2);
    client->parser.attributes.XOR_PEER_ADDRESS.port ^= *((N_16*)STUN_COOKIE);

    if(client->parser.attributes.XOR_PEER_ADDRESS.family == 1)
    {
        client->parser.attributes.XOR_PEER_ADDRESS.family = IPv4;
        read_byte_array(&client->STUN_client.buffer_input, &client->parser.attributes.XOR_PEER_ADDRESS.ip_v4, 4);
        client->parser.attributes.XOR_PEER_ADDRESS.ip_v4 ^= *((N_32*)STUN_COOKIE);
    }
    else if(client->parser.attributes.XOR_PEER_ADDRESS.family == 2)
    {
        client->parser.attributes.XOR_PEER_ADDRESS.family = IPv6;

        for(i=0; i<4; ++i)
            client->parser.attributes.XOR_PEER_ADDRESS.ip_v6[i] = read_binary_N_8(&client->STUN_client.buffer_input) ^ STUN_COOKIE[i];

        for(; i<16; ++i)
            client->parser.attributes.XOR_PEER_ADDRESS.ip_v6[i] = read_binary_N_8(&client->STUN_client.buffer_input) ^ (client->STUN_client.parser.head.transaction_id[i - 4]);
    }
}


private procedure add_XOR_PEER_ADDRESS_attribute (TURN_Client *client, Address *peer_address)
{
    N_32 i;

    if(peer_address->family == IPv4)
        add_STUN_atribute_head(&client->STUN_client, XOR_PEER_ADDRESS_TURN_ATTRIBUTE, 8);
    else if(peer_address->family == IPv6)
        add_STUN_atribute_head(&client->STUN_client, XOR_PEER_ADDRESS_TURN_ATTRIBUTE, 20);

    write_binary_N_8(&client->STUN_client.buffer_output, 0);

    if(peer_address->family == IPv4)
        write_binary_N_8(&client->STUN_client.buffer_output, 1);
    else if(peer_address->family == IPv6)
        write_binary_N_8(&client->STUN_client.buffer_output, 2);

    for(i=0; i<2; ++i)
        write_binary_N_8(&client->STUN_client.buffer_output, ((N_8*)&peer_address->port)[i] ^ STUN_COOKIE[i]);

    if(peer_address->family == IPv4)
    {
        for(i=0; i<4; ++i)
            write_binary_N_8(&client->STUN_client.buffer_output, ((N_8*)&peer_address->ip_v4)[i] ^ STUN_COOKIE[i]);
    }
    else if(peer_address->family == IPv6)
    {
        for(i=0; i<4; ++i)
            write_byte(&client->STUN_client.buffer_output, peer_address->ip_v6[i] ^ STUN_COOKIE[i]);

        for(; i<16; ++i)
            write_byte(&client->STUN_client.buffer_output, peer_address->ip_v6[i] ^ client->STUN_client.parser.head.transaction_id[i - 4]);
    }
}
