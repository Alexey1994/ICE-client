#ifdef __WIN32__
# include "windows network server.c"
#endif

#ifdef __linux__
# include "linux network server.c"
#endif