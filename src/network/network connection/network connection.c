#ifdef __WIN32__
# include "Windows network connection.c"
#endif

#ifdef __ANDROID_API__
# include "Android network connection.c"
#endif

#ifdef __linux__
# include "Linux network connection.c"
#endif