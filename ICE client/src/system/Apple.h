#ifndef APPLE_H_INCLUDED
#define APPLE_H_INCLUDED


#define __APPLE__


#define import   __attribute__((dllimport))
#define export   __attribute__((dllexport))
#define stdcall  __attribute__((__stdcall__))
#define cdecl    __attribute__((__cdecl__))


#include "extends/extends.h"


#endif //APPLE_H_INCLUDED