#ifndef LOGGER_H_INCLUDED
#define LOGGER_H_INCLUDED


void set_log_function(void(*print_message)(char *message));
void print_in_log(char *message);


#include "logger.c"

#endif //LOGGER_H_INCLUDED