#include "../../extends/extends.h"


String* create_string(int reserve)
{
    String *string=new(String);

    string->begin   = malloc(reserve);
    string->reserve = reserve;
    string->length  = 0;

    *string->begin='\0';

    return string;
}


void destroy_string(String *string)
{
    free(string->begin);
    free(string);
}


void push_in_string(String *string, Character character)
{
    if(string->length+1 == string->reserve)
    {
        string->reserve *= 2;
        string->begin = realloc(string->begin, string->reserve);
    }

    string->begin[string->length]=character;
    string->begin[string->length+1]='\0';

    string->length++;
}


Character pop_from_string(String *string)
{
    Character top;

    string->length--;
    top=string->begin[string->length];
    string->begin[string->length]='\0';

    return top;
}


void clear_string(String *string)
{
    string->length=0;
    *string->begin='\0';
}


void copy_char_array_to_string(String *string, Character *char_array)
{
    clear_string(string);

    for(; *char_array; char_array++)
        push_in_string(string, *char_array);
}


void copy_string_to_string(String *string1, String *string2)
{
    copy_char_array_to_string(string1, string2->begin);
}


void concatenate_string_with_char_array(String *string, Character *char_array)
{
    for(; *char_array; ++char_array)
        push_in_string(string, *char_array);
}


void concatenate_strings(String *string1, String *string2)
{
    concatenate_string_with_char_array(string1, string2->begin);
}


unsigned int convert_string_to_unsigned_int(String *string)
{
    Character *begin    = string->begin;
    unsigned int number = 0;

    for(; *begin && is_number(*begin); begin++)
        number = number * 10 + (*begin - '0');

    return number;
}


int convert_string_to_int(String *string)
{
    Character *begin  = string->begin;
    int number        = 0;
    Boolean is_negate = 0;

    if(*begin=='-')
    {
        begin++;
        is_negate=1;
    }

    for(; *begin && is_number(*begin); begin++)
        number = number * 10 + (*begin - '0');

    if(is_negate)
        return -number;

    return number;
}


Character* convert_string_to_char_array(String *string)
{
    Character *char_array = string->begin;
    free(string);
    return char_array;
}


int compare_strings(String *string1, String *string2)
{
    Character *iterator1 = string1->begin;
    Character *iterator2 = string2->begin;

    for(; *iterator1 && *iterator2; iterator1++, iterator2++)
        if(*iterator1 != *iterator2)
            break;

    return *iterator1 - *iterator2;
}


void write_byte_array_to_string(String *string, Byte *array, int length)
{
    for(; length; --length, ++array)
        push_in_string(string, *array);
}


void print_string(String *string)
{
    printf("%s", string->begin);
}

/*
String *str_init(const char *c)
{
    String *s=malloc(sizeof(String));
    char *i;

    s->begin=malloc(STRING_BUF_RESERVE+STRING_BUF_RESERVE);
    s->begin+=STRING_BUF_RESERVE;

    s->reserve_count_end=STRING_BUF_RESERVE;
    s->reserve_count_begin=STRING_BUF_RESERVE;
    s->length=0;
    s->length_begin=0;

    i=s->begin;
    while(*c)
    {
        s->length++;
        if(s->length==s->reserve_count_end)
        {
            *i='\0';
            s->reserve_count_end+=STRING_BUF_RESERVE;
            str_realloc(s);
            i=s->end;
        }

        *i=*c;
        i++;
        c++;
    }
    *i='\0';
    s->end=i;

    return s;
}


void str_push(String *s, char data)
{
    s->length++;
    if(s->reserve_count_end-s->length_begin==s->length)
    {
        s->reserve_count_end+=STRING_BUF_RESERVE;
        str_realloc(s);
    }

    *s->end=data;
    s->end++;
    *s->end='\0';
}


void str_push_back(String *s, char data)
{
    s->length_begin++;
    s->length++;
    if(s->reserve_count_end-s->length_begin==s->length)
    {
        s->reserve_count_begin+=STRING_BUF_RESERVE;
        str_realloc(s);
    }

    s->begin--;
    *s->begin=data;
}


char str_pop(String *s)
{
    char ret;

    s->end--;
    ret=*s->end;
    *s->end='\0';
    s->length--;

    return ret;
}


char str_pop_back(String *s)
{
    char ret=*s->begin;

    s->begin++;
    s->length_begin--;
    s->length--;

    return ret;
}


void str_clear(String *s)
{
    free(s->begin-s->reserve_count_begin+s->length_begin);

    s->begin=malloc(STRING_BUF_RESERVE+STRING_BUF_RESERVE);
    s->begin+=STRING_BUF_RESERVE;

    s->reserve_count_end=STRING_BUF_RESERVE;
    s->reserve_count_begin=STRING_BUF_RESERVE;
    s->length=0;
    s->length_begin=0;

    s->end=s->begin;
    *s->begin='\0';
}


String* str_copy(String *str)
{
    String *new_str=str_init("");
    int     i;

    for(i=0; i<str->length; i++)
        str_push(new_str, str->begin[i]);

    return new_str;
}


void str_concat(String *s1, String *s2)
{

}


void str_free(String *s)
{
    free(s->begin - s->reserve_count_begin + s->length_begin);
    free(s);
}


void str_print(String *s)
{
    int i;
    for(i=0; i<s->length; i++)
        printf("%c", s->begin[i]);
}


int str_to_int(String *s)
{
    return atoi(s->begin);
}


float str_to_float(String *s)
{
    return atof(s->begin);
}


int str_hex_to_int(String *s)
{
    int   hex_num=0,
          hex_byte;
    char *string;

    for(string=s->begin; *string; string++)
    {
        hex_byte=*string;

        if(is_number(hex_byte))
            hex_byte=hex_byte-'0';
        else
            hex_byte=hex_byte-'a'+10;

        hex_num=(hex_num<<4)|hex_byte;
    }

    return hex_num;
}


int str_comparision(String *s1, String *s2)
{
    return strcmp(s1->begin, s2->begin);
}


char* to_char_array(String *s)
{
    char *char_array=s->begin;
    free(s);
    return char_array;
}
*/
