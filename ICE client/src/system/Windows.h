#ifndef WINDOWS_H_INCLUDED
#define WINDOWS_H_INCLUDED


#define __WIN32__


#define import   __attribute__((dllimport))
#define export   __attribute__((dllexport))
#define stdcall  __attribute__((__stdcall__))
#define cdecl    __attribute__((__cdecl__))


#include "extends/extends.h"


#endif // WINDOWS_H_INCLUDED
