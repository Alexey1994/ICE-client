#include "network/network info/network info.h"
#include "network/TURN/TURN.h"

#ifdef __ANDROID_API__
# include "Android main.c"
#endif

#ifdef __WIN32__
# include "Windows main.c"
#endif

#ifdef __linux__
# include "Linux main.c"
#endif

