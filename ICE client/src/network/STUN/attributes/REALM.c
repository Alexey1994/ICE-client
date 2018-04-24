private procedure read_REALM_attribute (STUN_Client *client)
{
    N_32 i;

    initialize_buffer(&client->parser.attributes.REALM, client->parser.attribute.length + 1);

    for(i=0; i<client->parser.attribute.length; ++i)
        write_in_buffer(&client->parser.attributes.REALM, read_byte(&client->buffer_input));
}


private procedure add_REALM_attribute (STUN_Client *client, Buffer *realm)
{
    N_32 i;

    add_STUN_atribute_head(client, REALM_STUN_ATTRIBUTE, buffer_length(realm));

    for(i=realm->begin_index; i<realm->end_index; ++i)
        write_byte(&client->buffer_output, realm->data[i]);

    while(buffer_length(&client->buffer) & 3)
        write_byte(&client->buffer_output, 0);
}
