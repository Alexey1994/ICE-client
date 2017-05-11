#include "logger.h"
#include <stdio.h>


static void (*print_log_message)(char *message) = 0;


void set_log_function(void(*print_message)(char *message))
{
    print_log_message = print_message;
}


void print_log(char *message)
{
    if(!print_log_message)
        print_log_message = printf;

    print_log_message(message);
}
