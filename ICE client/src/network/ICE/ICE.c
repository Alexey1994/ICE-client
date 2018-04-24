private function Boolean send_address(ICE_Channel *channel, Address *local_address)
{
    Buffer       json_buffer;
    Output       json_output;
    JSON         json;
    Base64_Coder encoder;
    N_32         i;
    Input        http_input;

    initialize_buffer(&json_buffer, 1);
    initialize_buffer_output(&json_buffer, &json_output);

    begin_JSON(&json, &json_output);
        begin_JSON_object(&json);
            add_JSON_object_field(&json, "address");
                begin_JSON_string(&json);
                    begin_base64_encoder(&encoder, &json_output, &write_byte);
                        for(i=0; i<sizeof(Address); ++i)
                            encode_base64(&encoder, ((Byte*)local_address)[i]);
                    end_base64_encoder(&encoder);
                end_JSON_string(&json);

            add_JSON_object_field(&json, "source");
                begin_JSON_string(&json);
                    begin_base64_encoder(&encoder, &json_output, &write_byte);
                        for(i=0; i<sizeof(Address); ++i)
                            encode_base64(&encoder, ((Byte*)channel->source)[i]);
                    end_base64_encoder(&encoder);
                end_JSON_string(&json);
        end_JSON_object(&json);
    end_JSON(&json);

    if(!initialize_HTTP_Client_POST_input(&channel->signal_server_client, "/connect", &json_buffer, &http_input))
        goto error;

    deinitialize_input(&http_input);
    deinitialize_buffer(&json_buffer);

    return 1;

error:
    deinitialize_buffer(&json_buffer);
    return 0;
}


private function Boolean disconnect(ICE_Channel *channel)
{
    Buffer       json_buffer;
    Output       json_output;
    JSON         json;
    Base64_Coder encoder;
    N_32         i;
    Input        http_input;

    initialize_buffer(&json_buffer, 1);
    initialize_buffer_output(&json_buffer, &json_output);

    begin_JSON(&json, &json_output);
        begin_JSON_object(&json);
            add_JSON_object_field(&json, "source");
                begin_JSON_string(&json);
                    begin_base64_encoder(&encoder, &json_output, &write_byte);
                        for(i=0; i<sizeof(Address); ++i)
                            encode_base64(&encoder, ((Byte*)channel->source)[i]);
                    end_base64_encoder(&encoder);
                end_JSON_string(&json);

            add_JSON_object_field(&json, "destination");
                begin_JSON_string(&json);
                    begin_base64_encoder(&encoder, &json_output, &write_byte);
                        for(i=0; i<sizeof(Address); ++i)
                            encode_base64(&encoder, ((Byte*)channel->destination)[i]);
                    end_base64_encoder(&encoder);
                end_JSON_string(&json);
        end_JSON_object(&json);
    end_JSON(&json);

    if(!initialize_HTTP_Client_POST_input(&channel->signal_server_client, "/disconnect", &json_buffer, &http_input))
        goto error;

    deinitialize_input(&http_input);
    deinitialize_buffer(&json_buffer);

    return 1;

error:
    deinitialize_buffer(&json_buffer);
    return 0;
}


private function Boolean get_address(ICE_Channel *channel, Address *client_address)
{
    Buffer        json_buffer;
    Output        json_output;
    JSON          json;
    Base64_Coder  encoder;
    Base64_Coder  decoder;
    N_32          i;
    Input         http_input;
    Static_Buffer address_buffer;

    initialize_buffer(&json_buffer, 1);
    initialize_buffer_output(&json_buffer, &json_output);

    begin_JSON(&json, &json_output);
        begin_JSON_object(&json);
            add_JSON_object_field(&json, "source");
                begin_JSON_string(&json);
                    begin_base64_encoder(&encoder, &json_output, &write_byte);
                        for(i=0; i<sizeof(Address); ++i)
                            encode_base64(&encoder, ((Byte*)channel->source)[i]);
                    end_base64_encoder(&encoder);
                end_JSON_string(&json);

            add_JSON_object_field(&json, "destination");
                begin_JSON_string(&json);
                    begin_base64_encoder(&encoder, &json_output, &write_byte);
                        for(i=0; i<sizeof(Address); ++i)
                            encode_base64(&encoder, ((Byte*)channel->destination)[i]);
                    end_base64_encoder(&encoder);
                end_JSON_string(&json);
        end_JSON_object(&json);
    end_JSON(&json);

    if(!initialize_HTTP_Client_POST_input(&channel->signal_server_client, "/get_candidate", &json_buffer, &http_input))
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

    return 1;

error:
    deinitialize_buffer(&json_buffer);
    return 0;
}

/*
private procedure keep_alive_ICE_Channel (ICE_Channel *channel)
{
    loop
    {
        sleep_thread(5000);

        if(!transmit_data_using_ICE_Channel(channel, "k", 1))
        {

        }
    }
}
*/

private function Z_32 compare_ICE_candidates (ICE_Candidate *candidate1, ICE_Candidate *candidate2)
{
    return candidate2->priority - candidate1->priority;
}


private procedure destroy_ICE_Candidate (ICE_Candidate **candidate)
{
    if((*candidate)->type == RELAY_ICE_CANDIDATE)
    {
        deinitialize_TURN_Channel(&(*candidate)->channel);
    }
    else if((*candidate)->type == REFLEXIVE_ICE_CANDIDATE)
    {
        deinitialize_STUN_channel(&(*candidate)->channel);
    }

    free_memory((*candidate));
}


internal function Boolean initialize_ICE_Channel(
    ICE_Channel *channel,
    ICE_Server  *servers,
    N_32         servers_length,

    Address     *source,
    Address     *destination,
    Address     *signal_server
)
{
    ICE_Server     *server;
    ICE_Candidate  *candidate;
    ICE_Candidate **best_candidate;
    N_32            i;

    channel->servers                  = servers;
    channel->servers_length           = servers_length;

    channel->source                   = source;
    channel->destination              = destination;
    channel->signal_server            = signal_server;

    channel->local_candidates         = 0;
    channel->length_local_candidates  = 0;

    channel->remote_candidates        = 0;
    channel->length_remote_candidates = 0;

    if(!initialize_HTTP_Client(&channel->signal_server_client, signal_server))
        goto error;

    initialize_priority_queue(&channel->candidates, sizeof(ICE_Candidate**), &compare_ICE_candidates, &destroy_ICE_Candidate);

    for(i=0; i<servers_length; ++i)
    {
        server = servers + i;

        if(server->type == TURN_SERVER)
        {
            candidate = new(ICE_Candidate);

            candidate->type = RELAY_ICE_CANDIDATE;
            candidate->priority = (5 << 24) + ((2 << 8) * 65536) + (255 - channel->candidates.length);

            if(*server->username)
            {
                if(initialize_TURN_Channel(&candidate->channel, &server->address, server->username, server->password, channel, &send_address, channel, &get_address))
                    add_in_priority_queue(&channel->candidates, &candidate);
            }
            else
            {
                if(initialize_TURN_Channel(&candidate->channel, &server->address, 0, 0, channel, &send_address, channel, &get_address))
                    add_in_priority_queue(&channel->candidates, &candidate);
            }
        }
        else if(server->type == STUN_SERVER)
        {
            candidate = new(ICE_Candidate);

            candidate->type = REFLEXIVE_ICE_CANDIDATE;
            candidate->priority = (100 << 24) + ((2 << 8) * 65536) + (255 - channel->candidates.length);

            if(initialize_STUN_channel(&candidate->channel, &server->address, channel, &send_address, channel, &get_address))
                add_in_priority_queue(&channel->candidates, &candidate);
        }
    }

    best_candidate = remove_from_priority_queue(&channel->candidates);

    if(best_candidate)
        channel->current_candidate = *best_candidate;
    else
        channel->current_candidate = 0;

    return 1;

error:
    return 0;
}


internal function Boolean transmit_data_using_ICE_Channel (ICE_Channel *channel, Byte *data, N_32 length)
{
    Buffer buffer;
    N_32   i;

    if(!length)
        goto error;

    if(!channel->current_candidate)
        goto error;

    if(channel->current_candidate->type == REFLEXIVE_ICE_CANDIDATE)
    {
        if(!transmit_data_to_STUN_client(&channel->current_candidate->channel, data, length))
            goto error;
    }
    else if(channel->current_candidate->type == RELAY_ICE_CANDIDATE)
    {
        initialize_buffer(&buffer, length);

        for(i=0; i<length; ++i)
            write_in_buffer(&buffer, data[i]);

        if(!transmit_data_to_TURN_client(&channel->current_candidate->channel, &buffer))
        //while(!transmit_data_to_TURN_client(&channel->current_candidate->channel, &buffer))
        {
            //deinitialize_ICE_Channel(channel);
            //initialize_ICE_Channel(channel, channel->servers, channel->servers_length, channel->source, channel->destination, channel->signal_server_host, channel->signal_server_port);
            deinitialize_buffer(&buffer);
            goto error;
        }

        deinitialize_buffer(&buffer);
    }

    return 1;

error:
    return 0;
}


internal function Boolean receive_data_using_ICE_Channel (ICE_Channel *channel, Byte *data, N_32 *length)
{
    Buffer buffer;
    N_32   i;

    if(!channel->current_candidate)
        goto error;

    if(channel->current_candidate->type == REFLEXIVE_ICE_CANDIDATE)
    {
        if(!receive_data_from_STUN_client(&channel->current_candidate->channel, data, length))
            goto error;
    }
    else if(channel->current_candidate->type == RELAY_ICE_CANDIDATE)
    {
        initialize_buffer(&buffer, 1);

        if(!receive_data_from_TURN_client(&channel->current_candidate->channel, &buffer))
        //while(!receive_data_from_TURN_client(&channel->current_candidate->channel, &buffer))
        {
            //deinitialize_ICE_Channel(channel);
            //initialize_ICE_Channel(channel, channel->servers, channel->servers_length, channel->source, channel->destination, channel->signal_server_host, channel->signal_server_port);
            deinitialize_buffer(&buffer);
            goto error;
        }

        if(*length < buffer_length(&buffer))
        {
            deinitialize_buffer(&buffer);
            *length = 0;
            goto error;
        }

        *length = buffer_length(&buffer);

        for(i=buffer.begin_index; i<buffer.end_index; ++i)
            data[i] = buffer.data[i];

        deinitialize_buffer(&buffer);
    }

    return 1;

error:
    return 0;
}


internal procedure deinitialize_ICE_Channel(ICE_Channel *channel)
{
    deinitialize_priority_queue(&channel->candidates);

    if(channel->current_candidate)
        destroy_ICE_Candidate(&channel->current_candidate);

    disconnect(channel);
    deinitialize_HTTP_Client(&channel->signal_server_client);
}
