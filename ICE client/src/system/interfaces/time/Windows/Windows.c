internal function N_32 current_time()
{
    Windows_Time time;

    GetLocalTime(&time);
    return time.millisecond;
}