#ifndef THREAD_H_INCLUDED
#define THREAD_H_INCLUDED


internal function  Byte* run_thread   (procedure (*thread_function)(Byte *arguments), Byte *arguments);
internal procedure       sleep_thread (N_32 milliseconds);


#include "thread.c"

#endif // THREAD_H_INCLUDED
