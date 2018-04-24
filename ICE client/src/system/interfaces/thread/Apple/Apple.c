internal function Byte* run_thread (procedure (*thread_function)(Byte *arguments), Byte *arguments)
{
    pthread_t thread_ID;

    pthread_create(&thread_ID, 0, thread_function, arguments);

    return thread_ID;
}


internal procedure sleep_thread (N_32 milliseconds)
{
    usleep(milliseconds * 1000);
}
