#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED


#include "../data structures/buffer/buffer.h"


typedef struct
{
    Byte              *source;
    Buffer             buffer;
    function Byte    (*read_byte)(Byte *source);
    function Boolean (*end_of_data)(Byte *source);
    procedure        (*destroy_source)(Byte *source);
}
Input;


internal procedure           initialize_input            (Input *input, Byte *source, function Byte (*read_byte)(Byte *source), function Boolean (*end_of_data)(Byte *source));
internal procedure           deinitialize_input          (Input *input);

function           Boolean   read_if_next                (Input *input, Byte *next);
internal procedure           skip_spaces                 (Input *input);
internal procedure           skip_line                   (Input *input);

internal function  Byte      read_byte                   (Input *input);
internal function  Byte      input_head                  (Input *input);
internal function  Boolean   end_of_input                (Input *input);

internal procedure           read_byte_array             (Input *input, Byte *array, N_32 length);
internal function  N_8       read_binary_N_8             (Input *input);
internal function  N_16      read_binary_N_16            (Input *input);
internal function  N_32      read_binary_N_32            (Input *input);

internal function  N_32      read_N_32                   (Input *input);

internal procedure           initialize_buffer_input     (Buffer *buffer, Input *input);


#include "input.c"

#endif // INPUT_H_INCLUDED
