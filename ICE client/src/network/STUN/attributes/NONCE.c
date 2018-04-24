private procedure read_NONCE_attribute (STUN_Client *client)
{
    N_32 i;

    initialize_buffer(&client->parser.attributes.NONCE, client->parser.attribute.length + 1);

    for(i=0; i<client->parser.attribute.length; ++i)
        write_in_buffer(&client->parser.attributes.NONCE, read_byte(&client->buffer_input));
}


private procedure add_NONCE_attribute (STUN_Client *client, Buffer *nonce)
{
    N_32 i;

    add_STUN_atribute_head(client, NONCE_STUN_ATTRIBUTE, buffer_length(nonce));

    for(i=nonce->begin_index; i<nonce->end_index; ++i)
        write_byte(&client->buffer_output, nonce->data[i]);

    while(buffer_length(&client->buffer) & 3)
        write_byte(&client->buffer_output, 0);
}
