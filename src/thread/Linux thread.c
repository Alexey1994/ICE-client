#include <pthread.h>


void run_thread(void(*function)(), void *arg)
{
    pthread_t thread;
    pthread_create(&thread, 0, function, arg);
}


void sleep_thread(int milliseconds)
{
    usleep(milliseconds*1000);
}