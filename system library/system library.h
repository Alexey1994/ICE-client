#ifndef SYSTEM_LIBRARY_H_INCLUDED
#define SYSTEM_LIBRARY_H_INCLUDED


#include "../extends/extends.h"

Byte* open_system_function(Byte *library_name, Byte *function_name);
int run_system_function(int(*f)(), int *args, int length);


#include "system library.c"

#endif // SYSTEM_LIBRARY_H_INCLUDED
