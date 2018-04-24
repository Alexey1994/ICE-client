internal function Byte* run_thread (procedure (*thread_function)(Byte *arguments), Byte *arguments)
{
    N_32 thread_id;

    CreateThread(0, 0, thread_function, arguments, 0, &thread_id);

    return thread_id;
}


internal procedure sleep_thread (N_32 milliseconds)
{
    Sleep(milliseconds);
}
