private procedure add_STUN_atribute_head(STUN_Client *client, N_16 type, N_16 length)
{
    write_binary_N_16(&client->buffer_output, type);
    write_binary_N_16(&client->buffer_output, length);
}


internal procedure initialize_STUN_Client(STUN_Client *client, UDP_Connection *connection)
{
    memset(client, 0, sizeof(STUN_Client));
    client->connection = connection;
    initialize_buffer(&client->buffer, 65536);
    initialize_buffer_output(&client->buffer, &client->buffer_output);
}

internal procedure deinitialize_STUN_Client(STUN_Client *client)
{
    deinitialize_buffer(&client->buffer);
}


private procedure set_STUN_message_length(STUN_Client *client, N_16 length)
{
    N_16 *old_length;

    old_length = client->buffer.data + 2;
    *old_length = length;
    convert_little_to_big_endian(old_length, 2);
}


#include "attributes/MAPPED_ADDRESS.c"
#include "attributes/ERROR_CODE.c"
#include "attributes/NONCE.c"
#include "attributes/REALM.c"
#include "attributes/USERNAME.c"
#include "attributes/MESSAGE_INTEGRITY.c"


private procedure initialize_long_authentication_key(Buffer *key, Byte *username, Buffer *realm, Byte *password)
{
    Output key_output;
    N_32   i;

    initialize_buffer_output(key, &key_output);

    write_null_terminated_byte_array(&key_output, username);
    write_byte(&key_output, ':');

    for(i=realm->begin_index; i<realm->end_index; ++i)
        write_byte(&key_output, realm->data[i]);

    write_byte(&key_output, ':');
    write_null_terminated_byte_array(&key_output, password);
}


private procedure begin_STUN_request(STUN_Client *client, N_16 type)
{
    N_32 i;

    clear_buffer(&client->buffer);

    write_binary_N_16(&client->buffer_output, type);
    write_binary_N_16(&client->buffer_output, 0);
    write_byte_array(&client->buffer_output, STUN_COOKIE, 4);

    for(i=0; i<12; ++i)
        write_byte(&client->buffer_output, random_number());
}


private function Boolean end_STUN_request(STUN_Client *client)
{
    set_STUN_message_length(client, buffer_length(&client->buffer) - 20);

    if(!write_in_UDP_Connection(client->connection, client->buffer.data, buffer_length(&client->buffer)))
        goto error;

    return 1;

error:
    return 0;
}


private function Boolean initialize_STUN_response(STUN_Client *client)
{
    N_32 i;
    N_32 length;

    length = client->buffer.reserve;

    if(!read_from_UDP_Connection(client->connection, client->buffer.data, &length))
        goto error;

    clear_buffer(&client->buffer);
    client->buffer.end_index = 20;

    initialize_buffer_input(&client->buffer, &client->buffer_input);

    client->parser.head.type   = read_binary_N_16(&client->buffer_input);
    client->parser.head.length = read_binary_N_16(&client->buffer_input);
    client->parser.head.cookie = read_binary_N_32(&client->buffer_input);

    for(i=0; i<12; ++i)
        client->parser.head.transaction_id[i] = read_byte(&client->buffer_input);

    deinitialize_input(&client->buffer_input);
    client->buffer.end_index += client->parser.head.length;
    initialize_buffer_input(&client->buffer, &client->buffer_input);

    client->parser.attributes.ERROR_CODE = 0;
    client->parser.attributes.NONCE.reserve = 0;
    client->parser.attributes.REALM.reserve = 0;

    return 1;

error:
    return 0;
}


private procedure deinitialize_STUN_response(STUN_Client *client)
{
    deinitialize_input(&client->buffer_input);
}


private function Boolean begin_STUN_response(STUN_Client *client)
{
    Z_32 i;
    Z_32 attribute_begin_position;
    Z_32 bytes_not_readed;

    if(!initialize_STUN_response(client))
        goto error;

    while(buffer_length(&client->buffer))
    {
        client->parser.attribute.type   = read_binary_N_16(&client->buffer_input);
        client->parser.attribute.length = read_binary_N_16(&client->buffer_input);

        attribute_begin_position = client->buffer.begin_index;

        switch(client->parser.attribute.type)
        {
            case MAPPED_ADDRESS_STUN_ATTRIBUTE: read_MAPPED_ADDRESS_attribute(client); break;
            case ERROR_CODE_STUN_ATTRIBUTE:     read_ERROR_CODE_attribute(client);     break;
            case NONCE_STUN_ATTRIBUTE:          read_NONCE_attribute(client);          break;
            case REALM_STUN_ATTRIBUTE:          read_REALM_attribute(client);          break;

            default:
                for(i=0; i<client->parser.attribute.length; ++i)
                    read_byte(&client->buffer_input);
        }

        bytes_not_readed = client->parser.attribute.length - (client->buffer.begin_index - attribute_begin_position) + 1;

        for(i=0; i<bytes_not_readed; ++i)
            read_byte(&client->buffer_input);

        while(client->parser.attribute.length & 3) //align 4
        {
            read_byte(&client->buffer_input);
            ++client->parser.attribute.length;
        }
    }

    deinitialize_STUN_response(client);

    return 1;

error:
    return 0;
}


private procedure end_STUN_response(STUN_Client *client)
{
    //free attributes

    client->parser.attributes.ERROR_CODE = 0;

    if(client->parser.attributes.NONCE.reserve)
        deinitialize_buffer(&client->parser.attributes.NONCE);

    if(client->parser.attributes.REALM.reserve)
        deinitialize_buffer(&client->parser.attributes.REALM.reserve);
}


private function Boolean get_STUN_MAPPED_ADDRES(STUN_Client *client, Address *address)
{
    N_32 i;

    begin_STUN_request(client, BINDING_REQUEST);

    if(!end_STUN_request(client))
        goto error;

    if(!begin_STUN_response(client))
        goto error;

        if(client->parser.attributes.MAPPED_ADDRESS.family == 1)
        {
            address->family = 1;
            address->ip_v4  = client->parser.attributes.MAPPED_ADDRESS.ip_v4;
            address->port   = client->parser.attributes.MAPPED_ADDRESS.port;
        }
        else
        {
            address->family = 2;

            for(i=0; i<16; ++i)
                address->ip_v6[i] = client->parser.attributes.MAPPED_ADDRESS.ip_v6[i];
        }

    end_STUN_response(client);

    return 1;

error:
    return 0;
}


private function Boolean get_mapped_address(UDP_Connection *connection, Address *mapped_address)
{
    N_32        i;
    STUN_Client client;

    initialize_STUN_Client(&client, connection);

        if(!get_STUN_MAPPED_ADDRES(&client, mapped_address))
            goto error;

        memcpy(mapped_address, &client.parser.attributes.MAPPED_ADDRESS, sizeof(Address));

    deinitialize_STUN_Client(&client);
    return 1;

error:
    return 0;
}


internal function Boolean initialize_STUN_channel(
    STUN_Channel *channel,

    Address *STUN_server_address,

    Byte *local_ID,
    procedure (*send_local_address)(Byte *local_ID, Address *local),

    Byte *remote_ID,
    procedure (*get_remote_address)(Byte *remote_ID, Address *remote)
)
{
    if(!initialize_UDP_Connection(&channel->connection, STUN_server_address, 100000))
        goto error;

/*
    if(!initialize_address(&channel->local_address, "0.0.0.0", "8002", UDP_PROTOCOL))
    {
        deinitialize_UDP_Connection(&channel->connection);
        goto error;
    }

    if(!set_UDP_Connection_source(&channel->connection, &channel->local_address))
    {
        deinitialize_UDP_Connection(&channel->connection);
        goto error;
    }
*/
    if(!get_mapped_address(&channel->connection, &channel->local_address))
    {
        deinitialize_UDP_Connection(&channel->connection);
        goto error;
    }

    send_local_address(local_ID, &channel->local_address);
    get_remote_address(remote_ID, &channel->remote_address);

    set_UDP_Connection_destination(&channel->connection, &channel->remote_address);

    return 1;

error:
    return 0;
}


internal procedure deinitialize_STUN_channel(STUN_Channel *channel)
{
    deinitialize_UDP_Connection(&channel->connection);
}


internal function Boolean transmit_data_to_STUN_client(STUN_Channel *channel, Byte *data, N_32 data_length)
{
    if(!write_in_UDP_Connection(&channel->connection, data, data_length))
        goto error;

    return 1;

error:
    return 0;
}


internal function Boolean receive_data_from_STUN_client(STUN_Channel *channel, Byte *data, N_32 *data_length)
{
    if(!read_from_UDP_Connection(&channel->connection, data, data_length))
        goto error;

    return 1;

error:
    return 0;
}
