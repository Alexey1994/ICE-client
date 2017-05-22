#ifndef THREAD_H_INCLUDED
#define THREAD_H_INCLUDED


void run_thread(void(*function)(), void *arg);
void sleep_thread(int milliseconds);


#include "thread.c"

#endif // THREAD_H_INCLUDED
