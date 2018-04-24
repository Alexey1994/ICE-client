internal procedure revert_bytes_order (Byte *array, N_32 length)
{
    N_32 i;
    Byte tmp;

    for(i = 0; i < length/2; ++i)
    {
        tmp = array[i];
        array[i] = array[length-i-1];
        array[length-i-1] = tmp;
    }
}


internal procedure swap (Byte *element1, Byte *element2, N_32 size_of_element)
{
    N_32 i;
    Byte temp_byte;

    for(i=0; i<size_of_element; ++i)
    {
        temp_byte = element1[i];
        element1[i] = element2[i];
        element2[i] = temp_byte;
    }
}