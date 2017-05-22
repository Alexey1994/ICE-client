#include <windows.h>


Byte* open_system_function(Byte *library_name, Byte *function_name)
{
    library_name = LoadLibrary(library_name);

    if(!library_name)
        return 0;

    return GetProcAddress(library_name, function_name);
}


int run_system_function(int(*f)(), int *args, int length)
{
    for(length--; length>=0; length--)
    {
        __asm__("sub   $4, %%esp\n"
                "movl  %0, (%%esp)"
                :
                :"r"(args[length])
               );
    }

    return f();
}