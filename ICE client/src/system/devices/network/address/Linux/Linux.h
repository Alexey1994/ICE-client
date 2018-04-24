#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>


typedef struct
{
    int                  flags;
    int                  family;
    int                  socket_type;
    int                  protocol;
    socklen_t            address_length;
    Address             *address;
    char                *name;
    struct Address_Info *next;
}
Address_Info;

#include "Linux.c"
