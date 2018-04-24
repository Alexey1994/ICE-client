procedure begin_HTTP_Client_write(Output *output, HTTP_Method request_method, Byte *request)
{
    switch(request_method)
    {
        case HTTP_GET:
            write_null_terminated_byte_array(output, "GET ");
            break;

        case HTTP_POST:
            write_null_terminated_byte_array(output, "POST ");
            break;

        default:
            write_null_terminated_byte_array(output, "GET ");
    }

    write_null_terminated_byte_array(output, request);
    write_null_terminated_byte_array(output, " HTTP/1.1\r\n");
}


procedure add_HTTP_Client_field(Output *output, Byte *field, Byte *value)
{
    write_null_terminated_byte_array(output, field);
    write_null_terminated_byte_array(output, ": ");
    write_null_terminated_byte_array(output, value);
    write_null_terminated_byte_array(output, "\r\n");
}


procedure end_HTTP_Client_write(Output *output)
{
    write_null_terminated_byte_array(output, "\r\n");
}


function N_32 begin_HTTP_Client_read(Input *input)
{
    N_32 status;

    if(
        !read_if_next(input, "HTTP/0.9") &&
        !read_if_next(input, "HTTP/1.0") &&
        !read_if_next(input, "HTTP/1.1") &&
        !read_if_next(input, "HTTP/2")
    )
        goto error;

    skip_spaces(input);
    status = read_N_32(input);
    skip_line(input);

    return status;

error:
    return 0;
}


function Boolean read_HTTP_Client_field(Input *input, Buffer *field, Buffer *value)
{
    clear_buffer(field);
    clear_buffer(value);

    if(read_if_next(input, "\r"))
        return 0;

    while(input_head(input) != ':')
    {
        write_in_buffer(field, input_head(input));
        read_byte(input);
    }

    read_byte(input);
    skip_spaces(input);

    while(!read_if_next(input, "\r\n"))
    {
        write_in_buffer(value, input_head(input));
        read_byte(input);
    }

    return 1;
}


procedure end_HTTP_Client_read(Input *input)
{
    while(!read_if_next(input, "\r"))
        read_byte(input);
}


procedure write_HTTP_Client_GET(Output *output, Byte *path, Address *server_address)
{
    Buffer address_buffer;
    Output address_buffer_output;

    initialize_buffer(&address_buffer, 10);
    initialize_buffer_output(&address_buffer, &address_buffer_output);
    write_address(&address_buffer_output, server_address);
    write_byte(&address_buffer_output, '\0');

    begin_HTTP_Client_write(output, HTTP_GET, path);
        add_HTTP_Client_field(output, "Host", address_buffer.data);
    end_HTTP_Client_write(output);

    deinitialize_buffer(&address_buffer);
}


function N_32 read_HTTP_Client_GET(Input *input, N_32 *content_length)
{
    N_32   status;
    Buffer field_name;
    Buffer field_value;
    Input  field_value_input;

    status = begin_HTTP_Client_read(input);

    if(!status)
        goto error;

    initialize_buffer(&field_name, 1);
    initialize_buffer(&field_value, 1);

        while(read_HTTP_Client_field(input, &field_name, &field_value))
        {
            if(!memcmp(field_name.data, "Content-Length", buffer_length(&field_name)))
            {
                initialize_buffer_input(&field_value, &field_value_input);
                *content_length = read_N_32(&field_value_input);
                deinitialize_input(&field_value_input);
            }
        }

    deinitialize_buffer(&field_name);
    deinitialize_buffer(&field_value);

    return status;

error:
    return 0;
}


procedure write_HTTP_Client_POST (Output *output, Byte *path, Buffer *data, Address *server_address)
{
    Buffer address_buffer;
    Output address_buffer_output;

    Buffer content_length_buffer;
    Output content_length_buffer_output;

    initialize_buffer(&address_buffer, 10);
    initialize_buffer_output(&address_buffer, &address_buffer_output);
    write_address(&address_buffer_output, server_address);
    write_byte(&address_buffer_output, '\0');

    initialize_buffer(&content_length_buffer, 10);
    initialize_buffer_output(&content_length_buffer, &content_length_buffer_output);
    write_N_32(&content_length_buffer_output, buffer_length(data));
    write_byte(&content_length_buffer_output, '\0');

    begin_HTTP_Client_write(output, HTTP_POST, path);
        add_HTTP_Client_field(output, "Host", address_buffer.data);
        add_HTTP_Client_field(output, "Content-Type", "application/json");
        add_HTTP_Client_field(output, "Content-Length", content_length_buffer.data);
    end_HTTP_Client_write(output);

    write_byte_array(output, data->data + data->begin_index, buffer_length(data));

    deinitialize_buffer(&address_buffer);
    deinitialize_buffer(&content_length_buffer);
}


function N_32 read_HTTP_Client_POST  (Input *input, N_32 *content_length)
{
    return read_HTTP_Client_GET(input, content_length);
}


private function Byte read_byte_from_TCP_Connection(TCP_Connection *connection)
{
    Byte data;

    if(!read_from_TCP_Connection(connection, &data, 1))
        return 0;

    return data;
}


function Boolean initialize_HTTP_Client(HTTP_Client *client, Address *server_address)
{
    client->server_address = server_address;

    if(!initialize_TCP_Connection(&client->connection, server_address, 0))
        goto error;

    return 1;

error:
    return 0;
}


    typedef struct
    {
        HTTP_Client *client;
        Input        TCP_input;
        N_32         data_length;
    }
    HTTP_Client_GET_Source;


    private function Byte read_byte_from_HTTP_Client_GET_Source(HTTP_Client_GET_Source *source)
    {
        --source->data_length;

        if(source->data_length)
            return read_byte(&source->TCP_input);
    }


    private function Boolean is_end_of_HTTP_Client_GET_Source_data(HTTP_Client_GET_Source *source)
    {
        return !source->data_length;
    }


    private procedure deinitialize_HTTP_Client_GET_input_source(HTTP_Client_GET_Source *source)
    {
        deinitialize_input(&source->TCP_input);
        free_memory(source);
    }


    function N_32 initialize_HTTP_Client_GET_input(HTTP_Client *client, Byte *path, Input *input)
    {
        Buffer                  buffer;
        Output                  buffer_output;
        N_32                    status;
        N_32                    content_length;
        HTTP_Client_GET_Source *source;

        initialize_buffer(&buffer, 10);
        initialize_buffer_output(&buffer, &buffer_output);

        write_HTTP_Client_GET(&buffer_output, path, client->server_address);
        write_in_TCP_Connection(&client->connection, buffer.data, buffer_length(&buffer));

        deinitialize_buffer(&buffer);


        source = new(HTTP_Client_GET_Source);
        source->client = client;

        initialize_input(&source->TCP_input, &client->connection, &read_byte_from_TCP_Connection, 0);
        status = read_HTTP_Client_GET(&source->TCP_input, &content_length);
        source->data_length = content_length;

        if(!status)
        {
            deinitialize_input(&source->TCP_input);
            goto error;
        }

        initialize_input(input, source, &read_byte_from_HTTP_Client_GET_Source, &is_end_of_HTTP_Client_GET_Source_data);
        input->destroy_source = &deinitialize_HTTP_Client_GET_input_source;

        return status;

    error:
        return 0;
    }


    function N_32 initialize_HTTP_Client_POST_input(HTTP_Client *client, Byte *path, Buffer *data, Input *input)
    {
        Buffer                  buffer;
        Output                  buffer_output;
        N_32                    status;
        N_32                    content_length;
        HTTP_Client_GET_Source *source;

        initialize_buffer(&buffer, 10);
        initialize_buffer_output(&buffer, &buffer_output);

        write_HTTP_Client_POST(&buffer_output, path, data, client->server_address);
        write_in_TCP_Connection(&client->connection, buffer.data, buffer_length(&buffer));

        deinitialize_buffer(&buffer);


        source = new(HTTP_Client_GET_Source);
        source->client = client;

        initialize_input(&source->TCP_input, &client->connection, &read_byte_from_TCP_Connection, 0);
        status = read_HTTP_Client_POST(&source->TCP_input, &content_length);
        source->data_length = content_length;

        if(!status)
        {
            deinitialize_input(&source->TCP_input);
            free_memory(source);
            goto error;
        }

        initialize_input(input, source, &read_byte_from_HTTP_Client_GET_Source, &is_end_of_HTTP_Client_GET_Source_data);
        input->destroy_source = &deinitialize_HTTP_Client_GET_input_source;

        return status;

    error:
        return 0;
    }


procedure deinitialize_HTTP_Client(HTTP_Client *client)
{
    deinitialize_TCP_Connection(&client->connection);
}
