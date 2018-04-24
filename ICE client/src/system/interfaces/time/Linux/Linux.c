internal function N_32 current_time()
{
    struct timeval time_value;

    gettimeofday(&time_value, 0);

    return time_value.tv_usec / 1000;
}