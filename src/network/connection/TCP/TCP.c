#ifdef __WIN32__
# include "Windows TCP.c"
#endif

#ifdef __linux__
# include "Linux TCP.c"
#endif