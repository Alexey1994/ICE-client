#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED


#include <extends/extends.h>
#include <data structures/character/character.h>
#include <data structures/buffer/buffer.h>


typedef struct
{
	Byte              *source;
	Buffer             buffer;
	function Byte    (*read_byte)(Byte *source);
	function Boolean (*end_of_data)(Byte *source);
	procedure        (*destroy_source)(Byte *source);
}
Input;


procedure           initialize_input            (Input *input, Byte *source, function Byte (*read_byte)(Byte *source), function Boolean (*end_of_data)(Byte *source));
procedure           deinitialize_input          (Input *input);

function  Byte      read_byte                   (Input *input);
function  Byte      input_head                  (Input *input);
function  Boolean   end_of_input                (Input *input);

function  Boolean   read_if_next                (Input *input, Character *next);

procedure           read_byte_array             (Input *input, Byte *array, N_32 length);
function  Character read_character              (Input *input);
function  N_8       read_binary_N_8             (Input *input);
function  N_16      read_binary_N_16            (Input *input);
function  N_32      read_binary_N_32            (Input *input);
function  R_32      read_binary_R_32            (Input *input);
function  N_32      read_N_32                   (Input *input);
function  Z_32      read_Z_32                   (Input *input);
function  R_32      read_R_32                   (Input *input);

procedure           skip_input_spaces           (Input *input);
procedure           skip_input_text_empty_lines (Input *input);

function  Input*  create_buffer_input      (Buffer *buffer);
procedure         initialize_buffer_input  (Buffer *buffer, Input *input);


#include "input.c"

#endif // INPUT_H_INCLUDED
