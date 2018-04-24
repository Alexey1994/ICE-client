private procedure read_ERROR_CODE_attribute (STUN_Client *client)
{
    N_32 i;

    read_byte(&client->buffer_input);
    read_byte(&client->buffer_input);
    read_byte(&client->buffer_input);

    client->parser.attributes.ERROR_CODE = read_binary_N_8(&client->buffer_input) * 100 + read_binary_N_8(&client->buffer_input);
}
