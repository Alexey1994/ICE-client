typedef struct
{
    N_16 year;
    N_16 month;
    N_16 day_of_week;
    N_16 day;
    N_16 hour;
    N_16 minute;
    N_16 second;
    N_16 millisecond;
}
Windows_Time;


private stdcall procedure GetLocalTime (Windows_Time *time);

#include "Windows.c"