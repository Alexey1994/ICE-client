private UDP_Server OpenVPN_ICE_Client_server;
private Boolean    OpenVPN_ICE_Client_server_initialized = 0;


private function Boolean listen_OpenVPN_client(OpenVPN_ICE_Server *server, Address *client_address)
{
    HTTP_Client   signal_server_client;
    Buffer        json_buffer;
    Output        json_output;
    JSON          json;
    Base64_Coder  encoder;
    Base64_Coder  decoder;
    N_32          i;
    Input         http_input;
    Static_Buffer address_buffer;

    initialize_HTTP_Client(&signal_server_client, server->signal_server_address);
    initialize_buffer(&json_buffer, 1);
    initialize_buffer_output(&json_buffer, &json_output);

    begin_JSON(&json, &json_output);
        begin_JSON_object(&json);
            add_JSON_object_field(&json, "id");
                begin_JSON_string(&json);
                    begin_base64_encoder(&encoder, &json_output, &write_byte);
                        for(i=0; i<sizeof(Address); ++i)
                            encode_base64(&encoder, ((Byte*)server->server_address)[i]);
                    end_base64_encoder(&encoder);
                end_JSON_string(&json);
        end_JSON_object(&json);
    end_JSON(&json);

    if(!initialize_HTTP_Client_POST_input(&signal_server_client, "/listen", &json_buffer, &http_input))
        goto error;

    initialize_static_buffer(&address_buffer, client_address, sizeof(Address));

    begin_base64_decoder(&decoder, &address_buffer, &write_in_static_buffer);
        while(!end_of_input(&http_input))
        {
            decode_base64(&decoder, input_head(&http_input));
            read_byte(&http_input);
        }
    end_base64_decoder(&decoder);

    deinitialize_input(&http_input);
    deinitialize_buffer(&json_buffer);
    deinitialize_HTTP_Client(&signal_server_client);

    return 1;

error:
    deinitialize_HTTP_Client(&signal_server_client);
    deinitialize_buffer(&json_buffer);
    return 0;
}


private function Boolean stop_listen(OpenVPN_ICE_Server *server)
{
    HTTP_Client  signal_server_client;
    Buffer       json_buffer;
    Output       json_output;
    JSON         json;
    Base64_Coder encoder;
    N_32         i;
    Input        http_input;

    initialize_HTTP_Client(&signal_server_client, server->signal_server_address);
    initialize_buffer(&json_buffer, 1);
    initialize_buffer_output(&json_buffer, &json_output);

    begin_JSON(&json, &json_output);
        begin_JSON_object(&json);
            add_JSON_object_field(&json, "id");
                begin_JSON_string(&json);
                    begin_base64_encoder(&encoder, &json_output, &write_byte);
                        for(i=0; i<sizeof(Address); ++i)
                            encode_base64(&encoder, ((Byte*)server->server_address)[i]);
                    end_base64_encoder(&encoder);
                end_JSON_string(&json);
        end_JSON_object(&json);
    end_JSON(&json);

    if(!initialize_HTTP_Client_POST_input(&signal_server_client, "/stop_listen", &json_buffer, &http_input))
        goto error;

    deinitialize_input(&http_input);
    deinitialize_buffer(&json_buffer);
    deinitialize_HTTP_Client(&signal_server_client);

    return 1;

error:
    deinitialize_HTTP_Client(&signal_server_client);
    deinitialize_buffer(&json_buffer);
    return 0;
}


typedef struct
{
    Address            client_address;
    OpenVPN_ICE_Client client;
}
OpenVPN_ICE_Server_Client;



private procedure receive_from_destination_to_OpenVPN_ICE_Server_Client(Address *address, Byte *data, N_32 data_length, OpenVPN_ICE_Server_Client *client)
{
    transmit_data_using_ICE_Channel(&client->client.channel, data, data_length);
}


private procedure send_from_OpenVPN_ICE_Server_Client_to_destination(OpenVPN_ICE_Server_Client *client)
{
    Byte data[UDP_SERVER_RECEIVE_BUFFER_SIZE];
    N_32 data_length;

    loop
    {
        data_length = UDP_SERVER_RECEIVE_BUFFER_SIZE;

        if(receive_data_using_ICE_Channel(&client->client.channel, data, &data_length))
            sendto(OpenVPN_ICE_Client_server.socket, data, data_length, 0, &client->client.output, sizeof(Address));
    }
}


internal function Boolean initialize_OpenVPN_ICE_Server_Client_channel (OpenVPN_ICE_Server_Client *client, ICE_Server *ice_servers, N_32 ice_servers_length, Address *ice_source, Address *ice_destination, Address *signal_server, Address *OpenVPN_address)
{
    initialize_address(&client->client.output, "127.0.0.1", "1194", UDP_PROTOCOL);

    client->client.input = OpenVPN_address;
    client->client.output_initialized = 0;

    if(!initialize_ICE_Channel(&client->client.channel, ice_servers, ice_servers_length, ice_source, ice_destination, signal_server))
        goto error;

    if(!OpenVPN_ICE_Client_server_initialized)
    {
        if(!initialize_UDP_Server(&OpenVPN_ICE_Client_server, client->client.input, &receive_from_destination_to_OpenVPN_ICE_Server_Client, client))
            goto error;

        OpenVPN_ICE_Client_server_initialized = 1;
    }

    run_thread(&send_from_OpenVPN_ICE_Server_Client_to_destination, client);

    return 1;

error:
    return 0;
}


private procedure deinitialize_OpenVPN_ICE_Server_Client_channel (OpenVPN_ICE_Server_Client *client)
{
    deinitialize_UDP_Server(&OpenVPN_ICE_Client_server);
    OpenVPN_ICE_Client_server_initialized = 0;
    deinitialize_ICE_Channel(&client->client.channel);
}


private procedure listen_clients(OpenVPN_ICE_Server *server)
{
    OpenVPN_ICE_Server_Client *client;
    N_32                       i;

    while(server->is_running)
    {
        client = new(OpenVPN_ICE_Server_Client);

        if(
            listen_OpenVPN_client(server, &client->client_address) &&
            initialize_OpenVPN_ICE_Server_Client_channel(
                client,
                server->ice_servers,
                server->ice_servers_length,
                server->server_address,
                &client->client_address,
                server->signal_server_address,
                server->OpenVPN_address
            )
        )
                write_byte_array(&server->clients_output, client, sizeof(OpenVPN_ICE_Server_Client*));
        else
                free_memory(client);
    }

    for(i = server->clients.begin_index; i < server->clients.end_index; i += sizeof(OpenVPN_ICE_Server_Client*))
    {
        client = server->clients.data + i;
        deinitialize_OpenVPN_ICE_Server_Client_channel(client);
    }

    deinitialize_buffer(&server->clients);
    stop_listen(server);
}


function Boolean initialize_OpenVPN_ICE_Server (OpenVPN_ICE_Server *server, Address *server_address, ICE_Server *ice_servers, N_32 ice_servers_length, Address *signal_server_address, Address *OpenVPN_address)
{
    server->ice_servers           = ice_servers;
    server->ice_servers_length    = ice_servers_length;
    server->signal_server_address = signal_server_address;
    server->server_address        = server_address;
    server->OpenVPN_address       = OpenVPN_address;

    initialize_buffer(&server->clients, sizeof(OpenVPN_ICE_Server_Client*));
    initialize_buffer_output(&server->clients, &server->clients_output);

    server->is_running = 1;
    run_thread(&listen_clients, server);

    return 1;

error:
    return 0;
}


procedure deinitialize_OpenVPN_ICE_Server (OpenVPN_ICE_Server *server)
{
    server->is_running = 0;
}
