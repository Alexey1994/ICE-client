#ifdef __WIN32__
# include "Windows network server.c"
#endif

#ifdef __linux__
# include "Linux network server.c"
#endif