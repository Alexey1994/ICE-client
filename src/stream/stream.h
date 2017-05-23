#ifndef STREAM_H_INCLUDED
#define STREAM_H_INCLUDED


#include "../extends/extends.h"
#include "../data structures/array/array.h"
#include "../data structures/character/character.h"


typedef struct
{
    Byte          head;
    Byte         *source;
    Byte        (*get_byte)(Byte *source);
    void        (*put_byte)(Byte *source, Byte data);
    Byte        (*end_of_data)(Byte *source);

    Array        *local_buffer;
    Array        *buffer;
    unsigned int  begin_buffer;

    unsigned int  read_count;
}
Stream;


void           get_stream_bytes(Stream *stream, Byte *bytes, unsigned int size);

Stream*        create_stream(Byte *source, Byte(*get_byte)(Byte *source), Byte(*end_of_data)(Byte *source));
Stream*        create_output_stream(Byte *source, Byte(*put_byte)(Byte *source, Byte data));
void           destroy_stream(Stream *stream);

Byte           get_stream_byte(Stream *stream);
void           put_stream_byte(Stream *stream, Byte data);
void           get_stream_array(Stream *stream, Byte *array, int length);

char           end_stream(Stream *stream);

void           skip_text_stream(Stream *stream);
unsigned int   get_text_stream_uint(Stream *stream);
float          get_text_stream_float(Stream *stream);

char           read_if_on_stream_head(Stream *stream, char *word);
char           read_bytes_if_on_stream_head(Stream *stream, char *bytes, unsigned int size);


unsigned int   get_bin_stream_uint(Stream *stream);

unsigned short get_bin_stream_ushort(Stream *stream);
short          get_bin_stream_short(Stream *stream);

void           skip_stream(Stream *stream, unsigned int length);

void           put_text_in_output_stream(Stream *output_stream, char *data);
void           write_data_in_output_stream(Stream *output_stream, Byte *data, int length);

void           write_text_in_output_stream(Stream *output_stream, Character *text);
void           write_int_in_text_output_stream(Stream *output_stream, int number);


#include "stream.c"

#endif // STREAM_H_INCLUDED
