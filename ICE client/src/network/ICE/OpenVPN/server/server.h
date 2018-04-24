#ifndef OPEN_VPN_ICE_SERVER_H_INCLUDED
#define OPEN_VPN_ICE_SERVER_H_INCLUDED


#include "../client/client.h"
#include "../../ICE.h"


typedef struct
{
    ICE_Server     *ice_servers;
    N_32            ice_servers_length;
    Address        *signal_server_address;
    Address        *server_address;
    Address        *OpenVPN_address;
    Boolean         is_running;
    Buffer          clients;
    Output          clients_output;
}
OpenVPN_ICE_Server;


function  Boolean initialize_OpenVPN_ICE_Server   (OpenVPN_ICE_Server *server, Address *server_address, ICE_Server *ice_servers, N_32 ice_servers_length, Address *signal_server_address, Address *OpenVPN_address);
procedure         deinitialize_OpenVPN_ICE_Server (OpenVPN_ICE_Server *server);


#include "server.c"

#endif //OPEN_VPN_ICE_SERVER_H_INCLUDED