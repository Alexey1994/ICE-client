#include "stream.h"


void get_stream_bytes(Stream *stream, Byte *bytes, unsigned int size)
{
    for(; size; --size, ++bytes)
    {
        *bytes = stream->head;
        get_stream_byte(stream);
    }
}


Byte get_stream_byte(Stream *stream)
{
    ++stream->read_count;

    if(stream->buffer->length)
    {
        stream->head=stream->buffer->data[stream->begin_buffer];
        stream->begin_buffer++;

        if(stream->begin_buffer==stream->buffer->length)
        {
            stream->begin_buffer=0;
            stream->buffer->length=0;
        }

        return stream->head;
    }

    stream->head=stream->get_byte(stream->source);

    return stream->head;
}


void get_stream_array(Stream *stream, Byte *array, int length)
{
    for(; length; length--, array++)
    {
        *array = stream->head;
        get_stream_byte(stream);
    }
}


void put_stream_byte(Stream *stream, Byte data)
{
    stream->put_byte(stream->source, data);
}


unsigned int get_bin_stream_uint(Stream *stream)
{
    unsigned int   num;
    unsigned char *num_iterrator=&num;
    int            i;

    for(i=3; i>=0; i--)
    {
        num_iterrator[i] = stream->head;
        get_stream_byte(stream);
    }

    return num;
}


unsigned short get_bin_stream_ushort(Stream *stream)
{
    unsigned short  num;
    int             i;

    unsigned char  *num_iterrator = &num;

    for(i=1; i>=0; i--)
    {
        num_iterrator[i] = stream->head;
        get_stream_byte(stream);
    }

    return num;
}

short get_bin_stream_short(Stream *stream)
{
    short num;
    int   i;

    unsigned char  *num_iterrator = &num;

    for(i=1; i>=0; i--)
    {
        num_iterrator[i] = stream->head;
        get_stream_byte(stream);
    }

    return num;
}


void skip_stream(Stream *stream, unsigned int length)
{
    for(; length; length--)
        get_stream_byte(stream);
}



Stream* create_stream(Byte *source, Byte(*get_byte)(Byte *source), Byte(*end_of_data)(Byte *source))
{
    Stream *stream=new(Stream);

    stream->source       = source;
    stream->get_byte     = get_byte;
    stream->end_of_data  = end_of_data;
    stream->buffer       = create_array(10, 0);
    stream->local_buffer = create_array(10, 0);
    stream->begin_buffer = 0;
    stream->read_count   = 0;

    get_stream_byte(stream);

    return stream;
}


Stream* create_output_stream(Byte *source, Byte(*put_byte)(Byte *source, Byte data))
{
    Stream *stream=new(Stream);

    stream->source       = source;
    stream->put_byte     = put_byte;
    stream->buffer       = 0;
    stream->local_buffer = 0;

    return stream;
}


void destroy_stream(Stream *stream)
{
    if(stream->buffer)
        destroy_array(stream->buffer);

    if(stream->local_buffer)
        destroy_array(stream->local_buffer);

    free(stream);
}


char end_stream(Stream *stream)
{
    if(stream->end_of_data(stream->source) && !stream->buffer->length)
        return 1;

    return 0;
}


char read_if_on_stream_head(Stream *stream, char *word)
{
    char          buffer_head=stream->head;
    int           i;

    if(stream->head!=*word || end_stream(stream))
        return 0;

    word++;

    for(; *word; word++)
    {
        get_stream_byte(stream);
        push_in_array(stream->local_buffer, stream->head);

        if(stream->head!=*word || end_stream(stream))
        {
            for(i=0; i<stream->local_buffer->length; i++)
                push_in_array(stream->buffer, stream->local_buffer->data[i]);

            stream->local_buffer->length=0;
            stream->head=buffer_head;
            return 0;
        }
    }

    get_stream_byte(stream);
    stream->local_buffer->length=0;

    return 1;
}


char read_bytes_if_on_stream_head(Stream *stream, char *bytes, unsigned int size)
{
    char          buffer_head=stream->head;
    int           i;

    if(stream->head!=*bytes || end_stream(stream))
        return 0;

    bytes++;
    size--;

    for(; size; bytes++, size--)
    {
        get_stream_byte(stream);
        push_in_array(stream->local_buffer, stream->head);

        if(stream->head!=*bytes || end_stream(stream))
        {
            for(i=0; i<stream->local_buffer->length; i++)
                push_in_array(stream->buffer, stream->local_buffer->data[i]);

            stream->local_buffer->length=0;
            stream->head=buffer_head;
            return 0;
        }
    }

    get_stream_byte(stream);
    stream->local_buffer->length=0;

    return 1;
}


void skip_text_stream(Stream *stream)
{
    while(is_space(stream->head))
        get_stream_byte(stream);
}


unsigned int get_text_stream_uint(Stream *stream)
{
    unsigned int number=0;

    if(!is_number(stream->head) || end_stream(stream))
        return 0;

    loop
    {
        number+=stream->head-'0';
        get_stream_byte(stream);

        if(!is_number(stream->head) || end_stream(stream))
            break;

        number*=10;
    }

    return number;
}


int get_text_stream_int(Stream *stream)
{
    int  number    = 0;
    char is_negate = 0;

    if(stream->head=='-')
    {
        is_negate=1;
        get_stream_byte(stream);
        skip_text_stream(stream);
    }

    number=get_text_stream_uint(stream);

    if(is_negate)
        return -number;

    return number;
}


float get_text_stream_float(Stream *stream)
{
    int    integer_part;
    float  decimal_part;
    float  pow_10;
    char   is_negate = 0;

    if(stream->head=='-')
    {
        is_negate=1;
        get_stream_byte(stream);
        skip_text_stream(stream);
    }

    integer_part=get_text_stream_uint(stream);

    if(stream->head!='.')
        return integer_part;

    get_stream_byte(stream);
    decimal_part=0;
    pow_10=10.0f;

    while(is_number(stream->head) && !end_stream(stream))
    {
        decimal_part+=(stream->head-'0')/pow_10;
        pow_10*=10.0f;
        get_stream_byte(stream);
    }

    if(is_negate)
        return - integer_part - decimal_part;

    return integer_part + decimal_part;
}


void put_text_in_output_stream(Stream *output_stream, char *data)
{
    for(; *data; ++data)
        put_stream_byte(output_stream, *data);
}


void write_data_in_output_stream(Stream *output_stream, Byte *data, int length)
{
    int i;

    for(i=0; i<length; ++i)
        put_stream_byte(output_stream, data[i]);
}


void write_text_in_output_stream(Stream *output_stream, Character *text)
{
    for(; *text; ++text)
        put_stream_byte(output_stream, *text);
}


static int get_number_text_length(int number)
{
    if(!number)
        return 1;

    int length = 0;

    for(; number; number /= 10)
        ++length;

    return length;
}


static int pow_10(int n)
{
    int result = 1;

    for(; n; --n)
        result *= 10;

    return result;
}


void write_int_in_text_output_stream(Stream *output_stream, int number)
{
    if(!number)
    {
        put_stream_byte(output_stream, number + '0');
        return;
    }

    int slicer = pow_10( get_number_text_length(number) - 1 );

    if(number < 0)
    {
        put_stream_byte(output_stream, '-');
        number = -number;
    }

    while(slicer)
    {
        put_stream_byte(output_stream, number/slicer%10 + '0');
        slicer /= 10;
    }
}
