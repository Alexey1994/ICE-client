#ifndef BUFFER_H_INCLUDED
#define BUFFER_H_INCLUDED


//#include "../../input/input.h"
#include "../../output/output.h"


typedef struct
{
    N_32    reserve;
    N_32    begin_index;
    N_32    end_index;
    Boolean end_of_buffer;
    Byte   *data;
}
Buffer;


internal procedure         initialize_buffer        (Buffer *buffer, N_32 reserve);
internal procedure         deinitialize_buffer      (Buffer *buffer);

internal procedure         initialize_buffer_copy   (Buffer *copy, Buffer *original);
internal procedure         copy_buffer              (Buffer *copy, Buffer *original);
internal procedure         add_buffer               (Buffer *a, Buffer *b);

internal function  N_32    buffer_length            (Buffer *buffer);

internal function  Boolean end_of_buffer            (Buffer *buffer);
internal function  Byte    read_from_buffer         (Buffer *buffer);
internal procedure         write_in_buffer          (Buffer *buffer, Byte byte);

internal procedure         initialize_buffer_output (Buffer *buffer, Output *output);

internal procedure         clear_buffer             (Buffer *buffer);


#include "buffer.c"

#endif // BUFFER_H_INCLUDED
