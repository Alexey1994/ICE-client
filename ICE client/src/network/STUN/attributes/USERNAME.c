private procedure add_USERNAME_attribute(STUN_Client *client, Byte *username)
{
    N_32 username_length;

    username_length = strlen(username);

    add_STUN_atribute_head(client, USERNAME_STUN_ATTRIBUTE, username_length);
    write_byte_array(&client->buffer_output, username, username_length);

    while(username_length & 3)
    {
        write_byte(&client->buffer_output, 0);
        ++username_length;
    }
}
