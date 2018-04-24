#ifndef BASE64_H_INCLUDED
#define BASE64_H_INCLUDED


typedef struct
{
    Byte        previouse_byte;
    N_8         position;
    Byte       *output;
    procedure (*write_byte)(Byte *output, Byte byte);
}
Base64_Coder;


internal procedure         begin_base64_encoder (Base64_Coder *encoder, Byte *output, procedure (*write_byte)(Byte *output, Byte byte));
internal procedure         encode_base64        (Base64_Coder *encoder, Byte byte);
internal procedure         end_base64_encoder   (Base64_Coder *encoder);

internal procedure         begin_base64_decoder (Base64_Coder *decoder, Byte *output, procedure (*write_byte)(Byte *output, Byte byte));
internal procedure         decode_base64        (Base64_Coder *decoder, Byte byte);
internal function  Boolean end_base64_decoder   (Base64_Coder *decoder);


#include "base64.c"

#endif //BASE64_H_INCLUDED