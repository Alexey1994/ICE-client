DynamicData* create_dynamic_data(Byte *data, Byte type)
{
    DynamicData *dynamic_data = new(DynamicData);

    dynamic_data->data = data;
    dynamic_data->type = type;

    return dynamic_data;
}


void destroy_dynamic_data(DynamicData *dynamic_data)
{
    free(dynamic_data);
}


void convert_big_to_little_endian(Byte *array, int length)
{
    int  i;
    Byte tmp;

    for(i = 0; i < length/2; ++i)
    {
        tmp = array[i];
        array[i] = array[length-i-1];
        array[length-i-1] = tmp;
    }
}
