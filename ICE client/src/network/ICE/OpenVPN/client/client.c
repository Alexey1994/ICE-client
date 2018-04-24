private function Boolean connect_to_OpenVPN_Server(Address *source_address, Address *destination_address, Address *signal_server_address)
{
    HTTP_Client  signal_server_client;
    Buffer       json_buffer;
    Output       json_output;
    JSON         json;
    Base64_Coder encoder;
    N_32         i;
    Input        http_input;

    initialize_HTTP_Client(&signal_server_client, signal_server_address);
    initialize_buffer(&json_buffer, 1);
    initialize_buffer_output(&json_buffer, &json_output);

    begin_JSON(&json, &json_output);
        begin_JSON_object(&json);
            add_JSON_object_field(&json, "source");
                begin_JSON_string(&json);
                    begin_base64_encoder(&encoder, &json_output, &write_byte);
                        for(i=0; i<sizeof(Address); ++i)
                            encode_base64(&encoder, ((Byte*)source_address)[i]);
                    end_base64_encoder(&encoder);
                end_JSON_string(&json);

            add_JSON_object_field(&json, "destination");
                begin_JSON_string(&json);
                    begin_base64_encoder(&encoder, &json_output, &write_byte);
                        for(i=0; i<sizeof(Address); ++i)
                            encode_base64(&encoder, ((Byte*)destination_address)[i]);
                    end_base64_encoder(&encoder);
                end_JSON_string(&json);
        end_JSON_object(&json);
    end_JSON(&json);

    if(!initialize_HTTP_Client_POST_input(&signal_server_client, "/connect_to", &json_buffer, &http_input))
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


private procedure send_from_OpenVPN_ICE_Client_to_destination(Address *address, Byte *data, N_32 data_length, OpenVPN_ICE_Client *client)
{
    if(!client->output_initialized)
    {
        memcpy(&client->output, address, sizeof(Address));
        client->output_initialized = 1;
    }

    if(!transmit_data_using_ICE_Channel(&client->channel, data, data_length))
        deinitialize_UDP_Server(&client->server);
}


private procedure receive_from_destination_to_OpenVPN_ICE_Client(OpenVPN_ICE_Client *client)
{
    Byte data[UDP_SERVER_RECEIVE_BUFFER_SIZE];
    N_32 data_length;

    loop
    {
        data_length = UDP_SERVER_RECEIVE_BUFFER_SIZE;

        if(receive_data_using_ICE_Channel(&client->channel, data, &data_length))
        {
            if(client->output_initialized)
                sendto(client->server.socket, data, data_length, 0, &client->output, sizeof(Address));
        }
    }
}


function Boolean initialize_OpenVPN_ICE_Client (OpenVPN_ICE_Client *client, ICE_Server *ice_servers, N_32 ice_servers_length, Address *ice_source, Address *ice_destination, Address *signal_server, Address *OpenVPN_address)
{
    if(!connect_to_OpenVPN_Server(ice_source, ice_destination, signal_server))
        goto error;

    client->input = OpenVPN_address;
    client->output_initialized = 0;

    if(!initialize_ICE_Channel(&client->channel, ice_servers, ice_servers_length, ice_source, ice_destination, signal_server))
        goto error;

    if(!initialize_UDP_Server(&client->server, client->input, &send_from_OpenVPN_ICE_Client_to_destination, client))
        goto error;

    run_thread(&receive_from_destination_to_OpenVPN_ICE_Client, client);

    return 1;

error:
    return 0;
}


procedure deinitialize_OpenVPN_ICE_Client (OpenVPN_ICE_Client *client)
{
    deinitialize_UDP_Server(&client->server);
    deinitialize_ICE_Channel(&client->channel);
}