function Buffer* create_buffer (N_32 reserve)
{
    Buffer *buffer;

    buffer = new(Buffer);
    initialize_buffer(buffer, reserve);

    return buffer;
}


procedure destroy_buffer (Buffer *buffer)
{
    deinitialize_buffer(buffer);
    free(buffer);
}


procedure initialize_buffer (Buffer *buffer, N_32 reserve)
{
    buffer->reserve       = reserve;
    buffer->data          = malloc(reserve);
    buffer->begin_index   = 0;
    buffer->end_index     = 0;
    buffer->end_of_buffer = 0;
}


procedure deinitialize_buffer (Buffer *buffer)
{
    free(buffer->data);
}


function N_32 buffer_length (Buffer *buffer)
{
    return buffer->end_index - buffer->begin_index;
}


function Boolean end_of_buffer(Buffer *buffer)
{
    return buffer->end_of_buffer;
}


function Byte read_from_buffer(Buffer *buffer)
{
    Byte current_byte;

    if(buffer->begin_index == buffer->end_index)
    {
        buffer->end_of_buffer = 1;
        return 0;
    }

    current_byte = buffer->data[buffer->begin_index];
    ++buffer->begin_index;

    return current_byte;
}


procedure write_in_buffer(Buffer *buffer, Byte byte)
{
    if(buffer->end_index >= buffer->reserve)
    {
        buffer->reserve *= 2;
        //reallocate_array(buffer->data, Byte, buffer->reserve);
        buffer->data = realloc(buffer->data, buffer->reserve);
    }

    buffer->data[buffer->end_index] = byte;
    ++buffer->end_index;
    buffer->end_of_buffer = 0;
}


procedure initialize_buffer_copy(Buffer *copy, Buffer *original)
{
    N_32 i;

    initialize_buffer(copy, 2);

    for(i=original->begin_index; i<original->end_index; ++i)
        write_in_buffer(copy, original->data[i]);
}


procedure add_buffer(Buffer *a, Buffer *b)
{
    N_32 i;

    for(i=b->begin_index; i<b->end_index; ++i)
        write_in_buffer(a, b->data[i]);
}


function Output* create_buffer_output(Buffer *buffer)
{
    Output *output;

    output = new(Output);
    initialize_buffer_output(buffer, output);

    return output;
}


procedure initialize_buffer_output(Buffer *buffer, Output *output)
{
    initialize_output(output, buffer, &write_in_buffer);
}


procedure clear_buffer(Buffer *buffer)
{
    buffer->begin_index = 0;
    buffer->end_index = 0;
}