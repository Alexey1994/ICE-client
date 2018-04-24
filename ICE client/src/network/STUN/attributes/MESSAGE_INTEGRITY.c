private procedure add_MESSAGE_INTEGRITY_attribute (STUN_Client *client, Buffer *key)
{
    N_32   i;
    Byte   HMAC_SHA1_hash[20];
    Byte   MD5_key[16];
    Buffer new_key;
    Buffer data;

    initialize_buffer(&data, 1);
    initialize_buffer(&new_key, 1);

    calculate_MD5_hash(key, MD5_key);

    for(i=0; i<16; ++i)
        write_in_buffer(&new_key, MD5_key[i]);

    set_STUN_message_length(client, buffer_length(&client->buffer) + 4);

    for(i=client->buffer.begin_index; i<client->buffer.end_index; ++i)
        write_in_buffer(&data, client->buffer.data[i]);

    calculate_HMAC_SHA_1_hash(&data, &new_key, HMAC_SHA1_hash);

    add_STUN_atribute_head(client, MESSAGE_INTEGRITY_STUN_ATTRIBUTE, 20);
    write_byte_array(&client->buffer_output, HMAC_SHA1_hash, 20);

    deinitialize_buffer(&new_key);
    deinitialize_buffer(&data);
}
