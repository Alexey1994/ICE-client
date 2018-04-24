internal procedure initialize_priority_queue (
    Priority_Queue *queue,
    N_32            size_of_element,
    function Z_32 (*compare_elements) (Byte *element1, Byte *element2),
    procedure     (*delete_element)   (Byte *element)
)
{
    queue->length           = 0;
    queue->size_of_element  = size_of_element;
    queue->compare_elements = compare_elements;
    queue->delete_element   = delete_element;
    initialize_buffer(&queue->data, size_of_element);
}


internal procedure deinitialize_priority_queue (Priority_Queue *queue)
{
    N_32 i;

    if(queue->delete_element)
    {
        for(i = queue->data.begin_index; i < queue->data.end_index; i += queue->size_of_element)
            queue->delete_element(queue->data.data + i);
    }

    deinitialize_buffer(&queue->data);
}


internal procedure add_in_priority_queue (Priority_Queue *queue, Byte *data)
{
    N_32  i;
    Byte *element1;
    Byte *element2;
    N_32  current_index;
    N_32  top_index;

    for(i=0; i<queue->size_of_element; ++i)
        write_in_buffer(&queue->data, data[i]);

    ++queue->length;

    current_index = queue->length - 1;

    while(current_index)
    {
        top_index = (current_index - 1) >> 1;

        element1 = queue->data.data + current_index * queue->size_of_element;
        element2 = queue->data.data + top_index * queue->size_of_element;

        if(queue->compare_elements(element1, element2) < 0)
            swap(element1, element2, queue->size_of_element);
        else
            break;

        current_index = top_index;
    }
}


internal function Byte* remove_from_priority_queue (Priority_Queue *queue)
{
    Byte *last_element;
    Byte *minimum_element;
    N_32  minimum_element_index;
    Byte *element1;
    Byte *element2;
    N_32  current_index;
    N_32  left_index;
    N_32  right_index;

    if(!queue->length)
        return 0;

    --queue->length;
    last_element = queue->data.data + queue->length * queue->size_of_element;
    swap(queue->data.data, last_element, queue->size_of_element);

    current_index = 0;

    loop
    {
        left_index = (current_index << 1) + 1;

        if(left_index < queue->length)
        {
            right_index = (current_index + 1) << 1;

            if(right_index < queue->length)
            {
                element1 = queue->data.data + left_index * queue->size_of_element;
                element2 = queue->data.data + right_index * queue->size_of_element;

                if(queue->compare_elements(element1, element2) < 0)
                {
                    minimum_element = element1;
                    minimum_element_index = left_index;
                }
                else
                {
                    minimum_element = element2;
                    minimum_element_index = right_index;
                }
            }
            else
            {
                minimum_element = queue->data.data + left_index * queue->size_of_element;
                minimum_element_index = left_index;
            }
        }
        else
            break;

        element1 = queue->data.data + current_index * queue->size_of_element;

        if(queue->compare_elements(element1, minimum_element) < 0)
            break;

        swap(element1, minimum_element, queue->size_of_element);
        current_index = minimum_element_index;
    }

    queue->data.end_index -= queue->size_of_element;

    return last_element;
}
