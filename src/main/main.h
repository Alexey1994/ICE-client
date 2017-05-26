#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED


#include "../logger/logger.h"


int main(int arguments_length, char **arguments);


#ifdef __ANDROID__
# include "Android main.c"
#endif

#endif //MAIN_H_INCLUDED