#ifndef ICE_H_INCLUDED
#define ICE_H_INCLUDED


#include "../../system/devices/network/connection/TCP/TCP.h"
#include "../STUN/STUN.h"
#include "../TURN/TURN.h"
#include "../HTTP/HTTP.h"
#include "../JSON/JSON.h"
#include "../base64/base64.h"
#include "../../data structures/static buffer/static buffer.h"
#include "../../data structures/priority queue/priority queue.h"


typedef enum
{
    STUN_SERVER = 1,
    TURN_SERVER = 2
}
ICE_Server_Types;


typedef enum
{
    HOST_ICE_CANDIDATE      = 1,
    REFLEXIVE_ICE_CANDIDATE = 2,
    RELAY_ICE_CANDIDATE     = 3
}
ICE_Candidate_Types;


typedef struct
{
    Byte    type;
    Byte    username[32];
    Byte    password[32];
    Address address;
}
ICE_Server;


typedef struct
{
    Address        address;
    UDP_Connection connection;
    N_32           priority;
    Byte           type;

    union
    {
        TURN_Channel turn_channel;
        STUN_Channel stun_channel;
    }
    channel;
}
ICE_Candidate;


typedef struct
{
    ICE_Server     *servers;
    N_32            servers_length;
    Address        *source;
    Address        *destination;
    Address        *signal_server;

    HTTP_Client     signal_server_client;

    ICE_Candidate  *local_candidates;
    N_32            length_local_candidates;

    ICE_Candidate  *remote_candidates;
    N_32            length_remote_candidates;

    Priority_Queue  candidates;
    ICE_Candidate  *current_candidate;
}
ICE_Channel;


internal function Boolean initialize_ICE_Channel(
    ICE_Channel *channel,
    ICE_Server  *servers,
    N_32         servers_length,

    Address     *source,
    Address     *destination,
    Address     *signal_server
);

internal function Boolean transmit_data_using_ICE_Channel (ICE_Channel *channel, Byte *data, N_32 length);
internal function Boolean receive_data_using_ICE_Channel  (ICE_Channel *channel, Byte *data, N_32 *length);

internal procedure deinitialize_ICE_Channel(ICE_Channel *channel);


#include "ICE.c"

#endif //ICE_H_INCLUDED
