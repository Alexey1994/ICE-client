#ifdef __WIN32__
# include "Windows network connection.c"
#endif

#ifdef __linux__
# include "Linux network connection.c"
#endif