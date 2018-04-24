internal procedure initialize_static_buffer (Static_Buffer *buffer, Byte *data, N_32 length)
{
    buffer->length     = length;
    buffer->data_index = 0;
    buffer->data       = data;
}


internal procedure write_in_static_buffer (Static_Buffer *buffer, Byte byte)
{
    buffer->data[buffer->data_index] = byte;

    if(buffer->data_index < buffer->length - 1)
        ++buffer->data_index;
}


internal procedure initialize_static_buffer_output (Static_Buffer *buffer, Output *output)
{
    initialize_output(output, buffer, &write_in_static_buffer);
}