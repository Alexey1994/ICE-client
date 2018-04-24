#ifndef LINUX_H_INCLUDED
#define LINUX_H_INCLUDED


#define __linux__


#define import   __attribute__((dllimport))
#define export   __attribute__((dllexport))
#define stdcall  __attribute__((__stdcall__))
#define cdecl    __attribute__((__cdecl__))


#include "extends/extends.h"


#endif //LINUX_H_INCLUDED