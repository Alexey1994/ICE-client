#include "../../../../interfaces/thread/thread.h"
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>


#define ANY_INTERNET_PROTOCOL     0

#define SOCKET_STREAM             1
#define SOCKET_DATAGRAMS          2

#define TCP_PROTOCOL              6
#define UDP_PROTOCOL              17


#include "Linux.c"
