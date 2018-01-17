#include <pthread.h>


void run_thread(void(*f)(), void *arg)
{
    pthread_t thread;
    pthread_create(&thread, 0, f, arg);
}


void sleep_thread(int milliseconds)
{
    usleep(milliseconds*1000);
}
