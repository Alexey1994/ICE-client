#include <stdio.h>


static void (*print_error_function)(char *error_string) = 0;


void print_error(char *error_string)
{
    if(!print_error_function)
        print_error_function = printf;

    print_error_function(error_string);
}


void set_error_function(void(*print_error)(char *error_string))
{
    print_error_function = print_error;
}
