#ifndef BUFFER_H_INCLUDED
#define BUFFER_H_INCLUDED


#include "../../extends/extends.h"
//#include <input/input.h>
#include "../../output/output.h"


typedef struct
{
	Byte   *data;
	N_32    reserve;
	N_32    begin_index;
	N_32    end_index;
	Boolean end_of_buffer;
}
Buffer;


function  Buffer* create_buffer            (N_32 reserve);
procedure         destroy_buffer           (Buffer *buffer);
procedure         initialize_buffer        (Buffer *buffer, N_32 reserve);
procedure         deinitialize_buffer      (Buffer *buffer);

procedure         initialize_buffer_copy   (Buffer *copy, Buffer *original);
procedure         add_buffer               (Buffer *a, Buffer *b);

//function  Input*  create_buffer_input      (Buffer *buffer);
//procedure         initialize_buffer_input  (Buffer *buffer, Input *input);

function  N_32    buffer_length            (Buffer *buffer);

function  Boolean end_of_buffer            (Buffer *buffer);
function  Byte    read_from_buffer         (Buffer *buffer);
procedure         write_in_buffer          (Buffer *buffer, Byte byte);

function  Output* create_buffer_output     (Buffer *buffer);
procedure         initialize_buffer_output (Buffer *buffer, Output *output);

procedure         clear_buffer             (Buffer *buffer);


#include "buffer.c"

#endif // BUFFER_H_INCLUDED
