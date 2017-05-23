#ifdef __WIN32__
# include "Windows thread.c"
#endif

#ifdef __linux__
# include "Linux thread.c"
#endif