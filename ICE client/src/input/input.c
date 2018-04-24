internal procedure initialize_input (Input *input, Byte *source, function Byte (*read_byte)(Byte *source), function Boolean (*end_of_data)(Byte *source))
{
    input->source         = source;
    input->read_byte      = read_byte;
    input->end_of_data    = end_of_data;
    input->destroy_source = 0;

    initialize_buffer(&input->buffer, 1);
}


internal procedure deinitialize_input (Input *input)
{
    if(input->destroy_source)
        input->destroy_source(input->source);

    deinitialize_buffer(&input->buffer);
}


internal function Byte input_head (Input *input)
{
    Byte head;

    if(buffer_length(&input->buffer) == 0)
        read_byte(input);

    return input->buffer.data[ input->buffer.begin_index ];
}


internal function Byte read_byte (Input *input)
{
    if(end_of_input(input) && buffer_length(&input->buffer) <= 1)
        return 0;

    if(buffer_length(&input->buffer) == 0)
        write_in_buffer(&input->buffer, input->read_byte(input->source));
    else if(buffer_length(&input->buffer) > 1)
    {
        ++input->buffer.begin_index;

        if(buffer_length(&input->buffer) == 1)
        {
            *input->buffer.data = input->buffer.data[ input->buffer.begin_index ];
            input->buffer.begin_index = 0;
            input->buffer.end_index = 1;
        }
    }
    else
        *input->buffer.data = input->read_byte(input->source);

    return input->buffer.data[ input->buffer.begin_index ];
}


internal function Boolean end_of_input (Input *input)
{
    if(!input->end_of_data)
        return 0;

    return input->end_of_data(input->source);
}


function Boolean read_if_next (Input *input, Byte *next)
{
    Buffer accumulator;
    Buffer buffer_copy;

    initialize_buffer(&accumulator, 1);

    for(; *next && !end_of_input(input); ++next)
    {
        if(end_of_input(input))
        {
            deinitialize_buffer(&accumulator);
            return 0;
        }

        if(input_head(input) != *next)
        {
            initialize_buffer_copy(&buffer_copy, &input->buffer);
            clear_buffer(&input->buffer);
            add_buffer(&input->buffer, &accumulator);
            add_buffer(&input->buffer, &buffer_copy);

            deinitialize_buffer(&buffer_copy);
            deinitialize_buffer(&accumulator);
            return 0;
        }

        write_in_buffer(&accumulator, input_head(input));
        read_byte(input);
    }

    deinitialize_buffer(&accumulator);
    return 1;
}


internal procedure skip_spaces(Input *input)
{
    while(!end_of_input(input) && (input_head(input) == ' ' || input_head(input) == '\t' || input_head(input) == '\b') )
        read_byte(input);

    if(input_head(input) == ' ' || input_head(input) == '\t' || input_head(input) == '\b')
        read_byte(input);
}


internal procedure skip_line(Input *input)
{
    while(!end_of_input(input) && input_head(input) != '\n')
        read_byte(input);

    if(input_head(input) == '\n')
        read_byte(input);
}


internal procedure read_byte_array (Input *input, Byte *array, N_32 length)
{
    N_32 i;

    array[0] = input_head(input);

    for(i=1; i<length; ++i)
    {
        read_byte(input);
        array[i] = input_head(input);
    }

    ++input->buffer.begin_index;

    if(buffer_length(&input->buffer) == 0)
        clear_buffer(&input->buffer);
}


internal function N_8 read_binary_N_8 (Input *input)
{
    N_8 number;
    read_byte_array(input, &number, 1);
    return number;
}


internal function N_16 read_binary_N_16 (Input *input)
{
    N_16 number;
    read_byte_array(input, &number, 2);
    convert_little_to_big_endian(&number, 2);
    return number;
}


internal function N_32 read_binary_N_32 (Input *input)
{
    N_32 number;
    read_byte_array(input, &number, 4);
    convert_little_to_big_endian(&number, 4);
    return number;
}


internal function N_32 read_N_32 (Input *input)
{
    N_32 number;

    number = 0;

    while(!end_of_input(input) && input_head(input) >= '0' && input_head(input) <= '9')
    {
        number = number*10 + input_head(input) - '0';
        read_byte(input);
    }

    return number;
}


internal procedure initialize_buffer_input(Buffer *buffer, Input *input)
{
    initialize_input(input, buffer, &read_from_buffer, &end_of_buffer);
}
