internal procedure begin_JSON(JSON *json, Output *output)
{
    json->output = output;
    initialize_buffer(&json->stack, 1);
}


    internal procedure begin_JSON_string(JSON *json)
    {
        write_byte(json->output, '"');
    }


    internal procedure end_JSON_string(JSON *json)
    {
        write_byte(json->output, '"');
    }


    internal procedure begin_JSON_array(JSON *json)
    {
        write_byte(json->output, '[');
        write_in_buffer(&json->stack, 0);
    }


        internal procedure add_JSON_array_element(JSON *json)
        {
            if(buffer_length(&json->stack) && json->stack.data[json->stack.end_index - 1])
                write_byte(json->output, ',');
            else
                json->stack.data[json->stack.end_index - 1] = 1;
        }


    internal procedure end_JSON_array(JSON *json)
    {
        if(json->stack.end_index != json->stack.begin_index)
            --json->stack.end_index;

        write_byte(json->output, ']');
    }


    internal procedure begin_JSON_object(JSON *json)
    {
        write_byte(json->output, '{');
        write_in_buffer(&json->stack, 0);
    }


        internal procedure add_JSON_object_field(JSON *json, Byte *field_name)
        {
            if(buffer_length(&json->stack) && json->stack.data[json->stack.end_index - 1])
                write_byte(json->output, ',');
            else
                json->stack.data[json->stack.end_index - 1] = 1;

            write_byte(json->output, '"');
            write_null_terminated_byte_array(json->output, field_name);
            write_byte(json->output, '"');
            write_byte(json->output, ':');
        }


    internal procedure end_JSON_object(JSON *json)
    {
        if(json->stack.end_index != json->stack.begin_index)
            --json->stack.end_index;

        write_byte(json->output, '}');
    }


internal procedure end_JSON(JSON *json)
{
    deinitialize_buffer(&json->stack);
}
