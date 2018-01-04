procedure initialize_input (Input *input, Byte *source, function Byte (*read_byte)(Byte *source), function Boolean (*end_of_data)(Byte *source))
{
    input->source         = source;
    input->read_byte      = read_byte;
    input->end_of_data    = end_of_data;
    input->destroy_source = 0;

    initialize_buffer(&input->buffer, 1);
}


procedure deinitialize_input (Input *input)
{
    if(input->destroy_source)
        input->destroy_source(input->source);

    deinitialize_buffer(&input->buffer);
}


function Byte input_head (Input *input)
{
    Byte head;

    if(buffer_length(&input->buffer) == 0)
        read_byte(input);

    return input->buffer.data[ input->buffer.begin_index ];
}


function Byte read_byte (Input *input)
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
        input->buffer.data[ input->buffer.begin_index ] = input->read_byte(input->source);

    return input->buffer.data[ input->buffer.begin_index ];
}


function Boolean end_of_input (Input *input)
{
    if(!input->end_of_data)
        return 0;

    return input->end_of_data(input->source);
}


function Boolean read_if_next (Input *input, Character *next)
{
    Buffer accumulator;
    Buffer buffer_copy;

    initialize_buffer(&accumulator, 1);

    for(; *next && !end_of_input(input); ++next)
    {
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


procedure read_byte_array (Input *input, Byte *array, N_32 length)
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


function N_8 read_binary_N_8 (Input *input)
{
    N_8 number;
    read_byte_array(input, &number, 1);
    return number;
}


function N_16 read_binary_N_16 (Input *input)
{
    N_16 number;
    read_byte_array(input, &number, 2);
    return number;
}


function N_32 read_binary_N_32 (Input *input)
{
    N_32 number;
    read_byte_array(input, &number, 4);
    return number;
}


procedure skip_input_spaces (Input *input)
{
    while(!end_of_input(input) && is_space(input_head(input)))
        read_byte(input);
}


procedure initialize_buffer_input(Buffer *buffer, Input *input)
{
    initialize_input(input, buffer, &read_from_buffer, &end_of_buffer);
}


/*
function Boolean read_if_next (Input *input, Character *next)
{
    Buffer accumulator;
    Buffer buffer_copy;

    initialize_buffer(&accumulator, 2);

    for(; *next && !end_of_input(input); ++next)
    {
        write_in_buffer(&accumulator, input->head);

        if(input->head != *next)
        {
            initialize_buffer_copy(&buffer_copy, &input->buffer);
            clear_buffer(&input->buffer);
            input->head = read_from_buffer(&accumulator);
            add_buffer(&input->buffer, &accumulator);
            add_buffer(&input->buffer, &buffer_copy);

            deinitialize_buffer(&buffer_copy);
            deinitialize_buffer(&accumulator);
            return 0;
        }

        read_byte(input);
    }

    deinitialize_buffer(&accumulator);

    return 1;
}
*/

/*
procedure read_byte (Input *input)
{
    Byte current_byte;

    current_byte = input->head;

    if(input->buffer.begin_index >= input->buffer.end_index)
        input->head = input->read_byte(input->source);
    else
    {
        input->head = input->buffer.data[input->buffer.begin_index];
        ++input->buffer.begin_index;

        if(input->buffer.begin_index == input->buffer.end_index)
        {
            input->buffer.begin_index = 0;
            input->buffer.end_index = 0;
        }
    }

    return current_byte;
}


function Byte input_head(Input *input)
{
    return input->head;
}


function Boolean end_of_input(Input *input)
{
    return input->end_of_data(input->source);
}


function Character read_character (Input *input)
{
    Character character;

    character = input->head;
    read_byte(input);

    return character;
}


function N_8 read_binary_N_8 (Input *input)
{
    N_8  number;

    number = input->head;
    read_byte(input);

    return number;
}


function N_16 read_binary_N_16 (Input *input)
{
    N_16  number;

    read_byte_array(input, &number, 2);

    return number;
}


function N_32 read_binary_N_32 (Input *input)
{
    N_32  number;

    read_byte_array(input, &number, 4);

    return number;
}


function R_32 read_binary_R_32 (Input *input)
{
    R_32  number;

    read_byte_array(input, &number, 4);

    return number;
}


procedure read_byte_array (Input *input, Byte *array, N_32 length)
{
    for(; length; --length, ++array)
    {
        *array = input->head;
        read_byte(input);
    }
}


function N_32 read_N_32 (Input *input)
{
    N_32 number;

    number = 0;

    while(is_number(input->head) && !end_of_input(input))
    {
        number = number*10 + input->head - '0';
        read_byte(input);
    }

    return number;
}


function Z_32 read_Z_32 (Input *input)
{
    if(input->head == '-')
    {
        read_byte(input);
        return -(Z_32)read_N_32(input);
    }

    return read_N_32(input);
}


function R_32 read_R_32 (Input *input)
{
    R_32 number;
    R_32 res;

    number = read_Z_32(input);

    if(input->head != '.')
        return number;

    read_byte(input);
    res = read_N_32(input);

    while(res >= 1.0f)
        res /= 10.0f;

    if(number > 0.0f)
        return number + res;
    else
        return number - res;
}


function Boolean read_if_next (Input *input, Character *next)
{
    Buffer accumulator;
    Buffer buffer_copy;

    initialize_buffer(&accumulator, 2);

    for(; *next && !end_of_input(input); ++next)
    {
        write_in_buffer(&accumulator, input->head);

        if(input->head != *next)
        {
            initialize_buffer_copy(&buffer_copy, &input->buffer);
            clear_buffer(&input->buffer);
            input->head = read_from_buffer(&accumulator);
            add_buffer(&input->buffer, &accumulator);
            add_buffer(&input->buffer, &buffer_copy);

            deinitialize_buffer(&buffer_copy);
            deinitialize_buffer(&accumulator);
            return 0;
        }

        read_byte(input);
    }

    deinitialize_buffer(&accumulator);

    return 1;
}


function Boolean read_if_next_byte_array (Input *input, Byte *next, N_32 length)
{
    Buffer accumulator;
    Buffer buffer_copy;
    N_32   i;

    initialize_buffer(&accumulator, 2);

    for(i=0; i<length && !end_of_input(input); ++i)
    {
        write_in_buffer(&accumulator, input->head);

        if(input->head != next[i])
        {
            initialize_buffer_copy(&buffer_copy, &input->buffer);
            clear_buffer(&input->buffer);
            input->head = read_from_buffer(&accumulator);
            add_buffer(&input->buffer, &accumulator);
            add_buffer(&input->buffer, &buffer_copy);

            deinitialize_buffer(&buffer_copy);
            deinitialize_buffer(&accumulator);
            return 0;
        }

        read_byte(input);
    }

    deinitialize_buffer(&accumulator);

    return 1;
}


procedure skip_input_spaces (Input *input)
{
    while(!end_of_input(input) && is_space(input->head))
        read_byte(input);
}


procedure skip_input_text_empty_lines (Input *input)
{
    while(!end_of_input(input) && input->head != '\r' && input->head != '\n')
        read_byte(input);

    while(!end_of_input(input) && (input->head == '\n' || input->head == '\r'))
        read_byte(input);
}


function Input* create_buffer_input(Buffer *buffer)
{
    Input *input;

    input = new(Input);
    initialize_buffer_input(buffer, input);

    return input;
}


procedure initialize_buffer_input(Buffer *buffer, Input *input)
{
    initialize_input(input, buffer, &read_from_buffer, &end_of_buffer);
}*/
