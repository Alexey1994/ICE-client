private procedure read_DATA_attribute(TURN_Client *client)
{
    N_32 i;

    initialize_buffer(&client->parser.attributes.DATA.data, client->STUN_client.parser.attribute.length + 1);

    for(i=0; i<client->STUN_client.parser.attribute.length; ++i)
        write_in_buffer(&client->parser.attributes.DATA.data, read_byte(&client->STUN_client.buffer_input));
}


private procedure add_DATA_attribute (TURN_Client *client, Address *peer_address, Buffer *data)
{
    N_32 i;

    add_STUN_atribute_head(&client->STUN_client, DATA_TURN_ATTRIBUTE, buffer_length(data));

    for(i=data->begin_index; i<data->end_index; ++i)
        write_byte(&client->STUN_client.buffer_output, data->data[i]);

    while(buffer_length(&client->STUN_client.buffer) & 3)
        write_byte(&client->STUN_client.buffer_output, 0);
}
