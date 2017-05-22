#ifndef DYNAMIC_STRING_H_INCLUDED
#define DYNAMIC_STRING_H_INCLUDED


#include "../character/character.h"


typedef struct
{
    Character *begin;
    int        reserve;
    int        length;
}
String;


String* create_string(int reserve);
void destroy_string(String *string);

void push_in_string(String *string, Character character);
Character pop_from_string(String *string);

void clear_string(String *string);
void copy_char_array_to_string(String *string, Character *char_array);
void copy_string_to_string(String *string1, String *string2);

unsigned int convert_string_to_unsigned_int(String *string);
int convert_string_to_int(String *string);
Character* convert_string_to_char_array(String *string);

void concatenate_string_with_char_array(String *string, Character *char_array);
void concatenate_strings(String *string1, String *string2);

void write_byte_array_to_string(String *string, Byte *array, int length);

void print_string(String *string);


#include "string.c"

#endif // DYNAMIC_STRING_H_INCLUDED
