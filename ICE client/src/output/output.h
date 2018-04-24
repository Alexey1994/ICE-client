#ifndef OUTPUT_H_INCLUDED
#define OUTPUT_H_INCLUDED


typedef struct
{
    Byte       *source;
    procedure (*write_byte)(Byte *source, Byte byte);
}
Output;


internal procedure initialize_output                (Output *output, Byte *source, procedure (*write_byte)(Byte *source, Byte byte));

internal procedure write_byte                       (Output *output, Byte byte);
internal procedure write_byte_array                 (Output *output, Byte *data, N_32 length);
internal procedure write_null_terminated_byte_array (Output *output, Byte *data);
internal procedure write_binary_N_32                (Output *output, N_32 number);
internal procedure write_binary_N_16                (Output *output, N_16 number);
internal procedure write_binary_N_8                 (Output *output, N_8 number);

internal procedure write_N_32                       (Output *output, N_32 number);


#include "output.c"

#endif // OUTPUT_H_INCLUDED
