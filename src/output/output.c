procedure initialize_output (Output *output, Byte *source, procedure (*write_byte)(Byte *source, Byte byte))
{
    output->source     = source;
    output->write_byte = write_byte;
}


procedure deinitialize_output (Output *output)
{

}


procedure write_byte (Output *output, Byte byte)
{
    output->write_byte(output->source, byte);
}


procedure write_character (Output *output, Character character)
{
    write_byte(output, character);
}


procedure write_N_32 (Output *output, N_32 number)
{
    N_32 number_copy;
    N_32 right_selector;

    number_copy    = number;
    right_selector = 1;

    for(;;)
    {
        number_copy /= 10;

        if(!number_copy)
            break;

        right_selector *= 10;
    }

    do
    {
        write_byte(output, number / right_selector % 10 + '0');
        right_selector /= 10;
    }
    while(right_selector);
}


procedure write_byte_array (Output *output, Byte *data, N_32 length)
{
    N_32 i;

    for(i=0; i<length; ++i)
        write_byte(output, data[i]);
}


procedure write_null_terminated_byte_array (Output *output, Byte *data)
{
    for(; *data; ++data)
        write_byte(output, *data);
}


procedure write_binary_N_32 (Output *output, N_32 number)
{
    write_byte_array(output, &number, 4);
}


procedure write_binary_N_16 (Output *output, N_16 number)
{
    write_byte_array(output, &number, 2);
}


procedure write_binary_N_8 (Output *output, N_8 number)
{
    write_byte_array(output, &number, 1);
}