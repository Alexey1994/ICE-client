#ifndef JSON_H_INCLUDED
#define JSON_H_INCLUDED

/*
typedef enum
{
    JSON_ARRAY,
    JSON_OBJECT
}
JSON_Types;*/


typedef struct
{
    Output *output;
    Buffer  stack;
}
JSON;


internal procedure begin_JSON(JSON *json, Output *output);
    internal procedure begin_JSON_string(JSON *json);
    internal procedure end_JSON_string(JSON *json);
    
    internal procedure begin_JSON_array(JSON *json);
    internal procedure end_JSON_array(JSON *json);
    
    internal procedure begin_JSON_object(JSON *json);
        internal procedure add_JSON_object_field(JSON *json, Byte *field_name);
    internal procedure end_JSON_object(JSON *json);
internal procedure end_JSON(JSON *json);


#include "JSON.c"

#endif //JSON_H_INCLUDED