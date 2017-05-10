#ifndef ERROR_STREAM_H_INCLUDED
#define ERROR_STREAM_H_INCLUDED

void set_error_function(void(*print_error)(char *error_string));
void print_error(char *error_string);

#endif // ERROR_STREAM_H_INCLUDED
