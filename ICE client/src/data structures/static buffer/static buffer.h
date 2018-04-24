#ifndef STATIC_BUFFER_H_INCLUDED
#define STATIC_BUFFER_H_INCLUDED


#include "../../output/output.h"


typedef struct
{
    N_32  length;
    N_32  data_index;
    Byte *data;
}
Static_Buffer;


internal procedure initialize_static_buffer        (Static_Buffer *buffer, Byte *data, N_32 length);
internal procedure write_in_static_buffer          (Static_Buffer *buffer, Byte byte);
internal procedure initialize_static_buffer_output (Static_Buffer *buffer, Output *output);


#include "static buffer.c"

#endif //STATIC_BUFFER_H_INCLUDED