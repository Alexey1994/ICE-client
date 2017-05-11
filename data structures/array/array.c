#include "array.h"
#include "../../extends/extends.h"


Array* create_array(int reserve, void (*destroy_node)(char *node))
{
    Array *array=new(Array);

    array->reserve       = reserve;
    array->buffer_length = reserve;
    array->data          = new_array(char*, reserve);
    array->length        = 0;
    array->destroy_node  = destroy_node;

    return array;
}


void push_in_array(Array *array, char *data)
{
    if(array->length==array->buffer_length)
    {
        array->buffer_length+=array->reserve;
        reallocate_array(array->data, char*, array->buffer_length);
    }

    array->data[array->length]=data;
    array->length++;
}


char* pop_from_array(Array *array)
{
    array->length--;
    return array->data[array->length];
}


char* get_array_top(Array *array)
{
    return array->data[array->length-1];
}


static void free_array_data(Array *array)
{
    int    i;
    int    length                    = array->length;
    char **data                      = array->data;
    void (*destroy_node)(char *node) = array->destroy_node;

    for(i=0; i<length; i++)
        destroy_node(data[i]);
}


void destroy_array(Array *array)
{
    if(array->destroy_node)
        free_array_data(array);

    free(array->data);
    free(array);
}


void crawl_array(Array *array, void(*crawl_func)(char *array_node))
{
    int    i;
    char **data   = array->data;
    int    length = array->length;

    for(i=0; i<length; i++)
        crawl_func(data[i]);
}


static Byte* get_array_structure_element(RandomAccessStructure *structure, int pos)
{
    Array *array = structure->structure;

    if(pos >= array->length)
    {
        printf("out of range in array (length of array is %d)\n", array->length);
        goto error;
    }

    return array->data[pos];

error:
    printf("error in get_array_structure_element\n");
    //printf("out of range in array (length of array is %d)\n", array->length);

    return 0;
}


static int get_array_structure_length(RandomAccessStructure *structure)
{
    Array *array = structure->structure;
    return array->length;
}


RandomAccessStructure* create_array_structure(Array *array)
{
    return create_random_access_structure(array, get_array_structure_element, get_array_structure_length);
}


void destroy_array_structure(RandomAccessStructure *structure)
{
    Array *array = structure->structure;

    destroy_random_random_access_structure(structure);
    destroy_array(array);
}
