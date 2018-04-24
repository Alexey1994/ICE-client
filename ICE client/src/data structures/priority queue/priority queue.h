#ifndef PRIORITY_QUEUE_H_INCLUDED
#define PRIORITY_QUEUE_H_INCLUDED


#include "../buffer/buffer.h"


typedef struct
{
    N_32            length;
    N_32            size_of_element;
    function Z_32 (*compare_elements) (Byte *element1, Byte *element2);
    procedure     (*delete_element)   (Byte *element);
    Buffer          data;
}
Priority_Queue;


internal procedure       initialize_priority_queue   (
    Priority_Queue *queue,
    N_32            size_of_element,
    function Z_32 (*compare_elements) (Byte *element1, Byte *element2),
    procedure     (*delete_element)   (Byte *element)
);
internal procedure       deinitialize_priority_queue (Priority_Queue *queue);

internal procedure       add_in_priority_queue       (Priority_Queue *queue, Byte *data);
internal function Byte*  remove_from_priority_queue  (Priority_Queue *queue);


#include "priority queue.c"

#endif //PRIORITY_QUEUE_H_INCLUDED