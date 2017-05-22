#ifndef ERROR_STREAM_H_INCLUDED
#define ERROR_STREAM_H_INCLUDED


#define TIMEOUT_ERROR 2


void set_error_function(void(*print_error)(char *error_string));
void print_error(char *error_string);


#include "error.c"

#endif // ERROR_STREAM_H_INCLUDED
