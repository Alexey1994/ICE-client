#include "attributes/REQUESTED_TRANSPORT.c"
#include "attributes/XOR_RELAYED_ADDRESS.c"
#include "attributes/XOR_PEER_ADDRESS.c"
#include "attributes/DONT_FRAGMENT.c"
#include "attributes/DATA.c"


private procedure initialize_TURN_Client(TURN_Client *client, UDP_Connection *connection)
{
    initialize_STUN_Client(&client->STUN_client, connection);
}

private procedure deinitialize_TURN_Client(TURN_Client *client)
{
    deinitialize_STUN_Client(&client->STUN_client);
}


private procedure begin_TURN_request(TURN_Client *client, N_16 type)
{
    begin_STUN_request(&client->STUN_client, type);
    client->parser.attributes.DATA.data.reserve = 0;
}


private function Boolean end_TURN_request(TURN_Client *client)
{
    if(client->parser.attributes.DATA.data.reserve)
        deinitialize_buffer(&client->parser.attributes.DATA.data);

    return end_STUN_request(&client->STUN_client);
}


private function Boolean begin_TURN_response(TURN_Client *client)
{
    Z_32 i;
    Z_32 attribute_begin_position;
    Z_32 bytes_not_readed;

    if(!initialize_STUN_response(&client->STUN_client))
        goto error;

    client->parser.attributes.DATA.data.reserve = 0;

    while(!end_of_input(&client->STUN_client.buffer_input))
    {
        client->STUN_client.parser.attribute.type   = read_binary_N_16(&client->STUN_client.buffer_input);
        client->STUN_client.parser.attribute.length = read_binary_N_16(&client->STUN_client.buffer_input);

        attribute_begin_position = client->STUN_client.buffer.begin_index;

        switch(client->STUN_client.parser.attribute.type)
        {
            case XOR_RELAYED_ADDRESS_TURN_ATTRIBUTE: read_XOR_RELAYED_ADDRESS_attribute(client);      break;
            case DATA_TURN_ATTRIBUTE:                read_DATA_attribute(client);                     break;
            case XOR_PEER_ADDRESS_TURN_ATTRIBUTE:    read_XOR_PEER_ADDRESS_attribute(client);         break;

            case ERROR_CODE_STUN_ATTRIBUTE:          read_ERROR_CODE_attribute(&client->STUN_client); break;
            case NONCE_STUN_ATTRIBUTE:               read_NONCE_attribute(&client->STUN_client);      break;
            case REALM_STUN_ATTRIBUTE:               read_REALM_attribute(&client->STUN_client);      break;
        }

        bytes_not_readed = client->STUN_client.parser.attribute.length - (client->STUN_client.buffer.begin_index - attribute_begin_position) + 1;

        for(i=0; i<bytes_not_readed; ++i)
            read_byte(&client->STUN_client.buffer_input);

        while(client->STUN_client.parser.attribute.length & 3) //align 4
        {
            read_byte(&client->STUN_client.buffer_input);
            ++client->STUN_client.parser.attribute.length;
        }
    }

    deinitialize_STUN_response(&client->STUN_client);

    return 1;

error:
    return 0;
}


private procedure end_TURN_response(TURN_Client *client)
{
    //free attributes

    end_STUN_response(&client->STUN_client);

    if(client->parser.attributes.DATA.data.reserve)
        deinitialize_buffer(&client->parser.attributes.DATA.data);
}


private function Boolean allocate_TURN(UDP_Connection *connection, N_32 *error_code, Address *relayed_address, Buffer *nonce, Buffer *realm)
{
    TURN_Client client;

    initialize_TURN_Client(&client, connection);

        begin_TURN_request(&client, ALLOCATE_TURN_MESSAGE);

            add_REQUESTED_TRANSPORT_attribute(&client, 0);

        if(!end_TURN_request(&client))
            goto error;

        if(!begin_TURN_response(&client))
            goto error;

            memcpy(relayed_address, &client.parser.attributes.XOR_RELAYED_ADDRESS, sizeof(Address));
            *error_code = client.STUN_client.parser.attributes.ERROR_CODE;

            if(client.STUN_client.parser.attributes.NONCE.reserve)
                copy_buffer(nonce, &client.STUN_client.parser.attributes.NONCE);

            if(client.STUN_client.parser.attributes.REALM.reserve)
                copy_buffer(realm, &client.STUN_client.parser.attributes.REALM);

        end_TURN_response(&client);

    deinitialize_TURN_Client(&client);

    return 1;

error:
    return 0;
}


private function Boolean allocate_TURN_with_credentials (UDP_Connection *connection, N_32 *error_code, Address *relayed_address, Buffer *nonce, Buffer *realm, Byte *username, Buffer *key)
{
    TURN_Client client;

    initialize_TURN_Client(&client, connection);

        begin_TURN_request(&client, ALLOCATE_TURN_MESSAGE);

            add_REQUESTED_TRANSPORT_attribute(&client, 0);
            add_USERNAME_attribute(&client.STUN_client, username);
            add_REALM_attribute(&client.STUN_client, realm);
            add_NONCE_attribute(&client.STUN_client, nonce);
            add_MESSAGE_INTEGRITY_attribute(&client.STUN_client, key);

        if(!end_TURN_request(&client))
            goto error;


        if(!begin_TURN_response(&client))
            goto error;

            memcpy(relayed_address, &client.parser.attributes.XOR_RELAYED_ADDRESS, sizeof(Address));
            *error_code = client.STUN_client.parser.attributes.ERROR_CODE;

        end_TURN_response(&client);

    deinitialize_TURN_Client(&client);

    return 1;

error:
    return 0;
}


private function Boolean create_TURN_permission (UDP_Connection *connection, N_32 *error_code, Address *peer_address)
{
    TURN_Client client;

    initialize_TURN_Client(&client, connection);

        begin_TURN_request(&client, CREATE_PERMISSION_TURN_MESSAGE);

            add_XOR_PEER_ADDRESS_attribute(&client, peer_address);

        if(!end_TURN_request(&client))
            goto error;


        if(!begin_TURN_response(&client))
            goto error;

            *error_code = client.STUN_client.parser.attributes.ERROR_CODE;

        end_TURN_response(&client);

    deinitialize_TURN_Client(&client);

    return 1;

error:
    return 0;
}


private function Boolean create_TURN_permission_with_credentials (UDP_Connection *connection, N_32 *error_code, Address *peer_address, Buffer *nonce, Buffer *realm, Byte *username, Buffer *key)
{
    TURN_Client client;

    initialize_TURN_Client(&client, connection);

        begin_TURN_request(&client, CREATE_PERMISSION_TURN_MESSAGE);

            add_XOR_PEER_ADDRESS_attribute(&client, peer_address);
            add_USERNAME_attribute(&client.STUN_client, username);
            add_REALM_attribute(&client.STUN_client, realm);
            add_NONCE_attribute(&client.STUN_client, nonce);
            add_MESSAGE_INTEGRITY_attribute(&client.STUN_client, key);

        if(!end_TURN_request(&client))
            goto error;


        if(!begin_TURN_response(&client))
            goto error;

            *error_code = client.STUN_client.parser.attributes.ERROR_CODE;

        end_TURN_response(&client);

    deinitialize_TURN_Client(&client);

    return 1;

error:
    return 0;
}


private function Boolean send_TURN_data (UDP_Connection *connection, Address *peer_address, Buffer *data)
{
    TURN_Client client;

    initialize_TURN_Client(&client, connection);

        begin_TURN_request(&client, SEND_TURN_MESSAGE);

            add_XOR_PEER_ADDRESS_attribute(&client, peer_address);
            add_DATA_attribute(&client, peer_address, data);

        if(!end_TURN_request(&client))
            goto error;

    deinitialize_TURN_Client(&client);

    return 1;

error:
    return 0;
}


private function Boolean get_TURN_data (UDP_Connection *connection, Address *peer_address, Buffer *data)
{
    TURN_Client client;
    N_32        i;

    initialize_TURN_Client(&client, connection);

        if(!begin_TURN_response(&client))
            goto error;

            memcpy(peer_address, &client.parser.attributes.XOR_PEER_ADDRESS, sizeof(Address));
            copy_buffer(data, &client.parser.attributes.DATA.data);

        end_TURN_response(&client);

    deinitialize_TURN_Client(&client);

    return 1;

error:
    return 0;
}


internal function Boolean initialize_TURN_Channel(
    TURN_Channel *channel,

    Address *TURN_server_address,

    Byte *username,
    Byte *password,

    Byte *local_ID,
    procedure (*send_local_address)(Byte *local_ID, Address *local),

    Byte *remote_ID,
    procedure (*get_remote_address)(Byte *remote_ID, Address *remote)
)
{
    Buffer   nonce;
    Buffer   realm;
    Buffer   key;
    Address  relayed_address;
    N_32     error_code;

    if(!initialize_UDP_Connection(&channel->connection, TURN_server_address, 10000))
        goto error_1;

    initialize_buffer(&nonce, 1);
    initialize_buffer(&realm, 1);
    initialize_buffer(&key, 1);

    if(!allocate_TURN(&channel->connection, &error_code, &relayed_address, &nonce, &realm))
        goto error;

    if(error_code == 401)
    {
        if(!username || !password)
            goto error;

        initialize_long_authentication_key(&key, username, &realm, password);

        if(!allocate_TURN_with_credentials(&channel->connection, &error_code, &relayed_address, &nonce, &realm, username, &key))
            goto error;

        if(error_code)
            goto error;

        send_local_address(local_ID, &relayed_address);
        get_remote_address(remote_ID, &channel->remote);

        if(!create_TURN_permission_with_credentials(&channel->connection, &error_code, &channel->remote, &nonce, &realm, username, &key))
            goto error;

        if(error_code)
            goto error;
    }
    else
    {
        send_local_address(local_ID, &relayed_address);
        get_remote_address(remote_ID, &channel->remote);

        if(!create_TURN_permission(&channel->connection, &error_code, &channel->remote))
            goto error;

        if(error_code)
            goto error;
    }

    deinitialize_buffer(&nonce);
    deinitialize_buffer(&realm);
    deinitialize_buffer(&key);

    return 1;

error:
    deinitialize_UDP_Connection(&channel->connection);

    deinitialize_buffer(&nonce);
    deinitialize_buffer(&realm);
    deinitialize_buffer(&key);

error_1:
    return 0;
}


internal procedure deinitialize_TURN_Channel(TURN_Channel *channel)
{
    deinitialize_UDP_Connection(&channel->connection);
}


internal function Boolean transmit_data_to_TURN_client(TURN_Channel *channel, Buffer *data)
{
    if(!send_TURN_data(&channel->connection, &channel->remote, data))
        goto error;

    return 1;

error:
    return 0;
}


internal function Boolean receive_data_from_TURN_client(TURN_Channel *channel, Buffer *data)
{
    Address sender_address;
    N_32    i;

    if(!get_TURN_data(&channel->connection, &sender_address, data))
        goto error;

    if(sender_address.family != channel->remote.family)
        goto error;

    if(sender_address.port != channel->remote.port)
        goto error;

    if(sender_address.family == IPv4)
    {
        if(sender_address.ip_v4 != channel->remote.ip_v4)
            goto error;
    }
    else if(sender_address.family == IPv6)
    {
        for(i=0; i<16; ++i)
        {
            if(sender_address.ip_v6[i] != channel->remote.ip_v6[i])
                goto error;
        }
    }
    else
        goto error;

    return 1;

error:
    clear_buffer(data);
    return 0;
}
