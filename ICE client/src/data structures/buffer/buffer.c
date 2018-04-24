internal procedure destroy_buffer (Buffer *buffer)
{
    deinitialize_buffer(buffer);
    free_memory(buffer);
}


internal procedure initialize_buffer (Buffer *buffer, N_32 reserve)
{
    buffer->reserve       = reserve;
    buffer->data          = allocate_memory(reserve);
    buffer->begin_index   = 0;
    buffer->end_index     = 0;
    buffer->end_of_buffer = 0;
}


internal procedure deinitialize_buffer (Buffer *buffer)
{
    free_memory(buffer->data);
}


internal function N_32 buffer_length (Buffer *buffer)
{
    return buffer->end_index - buffer->begin_index;
}


internal function Boolean end_of_buffer(Buffer *buffer)
{
    return buffer->end_of_buffer;
}


internal function Byte read_from_buffer(Buffer *buffer)
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


internal procedure write_in_buffer(Buffer *buffer, Byte byte)
{
    if(buffer->end_index >= buffer->reserve)
    {
        buffer->reserve <<= 1;
        reallocate_array(buffer->data, Byte, buffer->reserve);
    }

    buffer->data[buffer->end_index] = byte;
    ++buffer->end_index;
    buffer->end_of_buffer = 0;
}


internal procedure initialize_buffer_copy(Buffer *copy, Buffer *original)
{
    N_32 i;

    initialize_buffer(copy, 2);

    for(i=original->begin_index; i<original->end_index; ++i)
        write_in_buffer(copy, original->data[i]);
}


internal procedure copy_buffer (Buffer *copy, Buffer *original)
{
    N_32 i;

    clear_buffer(copy);

    for(i=original->begin_index; i<original->end_index; ++i)
        write_in_buffer(copy, original->data[i]);
}


internal procedure add_buffer(Buffer *a, Buffer *b)
{
    N_32 i;

    for(i=b->begin_index; i<b->end_index; ++i)
        write_in_buffer(a, b->data[i]);
}


internal procedure initialize_buffer_output(Buffer *buffer, Output *output)
{
    initialize_output(output, buffer, &write_in_buffer);
}


internal procedure clear_buffer(Buffer *buffer)
{
    buffer->begin_index = 0;
    buffer->end_index = 0;
}
