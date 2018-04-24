#ifndef OPEN_VPN_ICE_CLIENT_H_INCLUDED
#define OPEN_VPN_ICE_CLIENT_H_INCLUDED


#include "../../../../system/devices/network/server/UDP/UDP.h"
#include "../../ICE.h"


typedef struct
{
    UDP_Server   server;
    Address     *input;
    Address      output;
    Boolean      output_initialized;
    ICE_Channel  channel;
}
OpenVPN_ICE_Client;


function  Boolean initialize_OpenVPN_ICE_Client   (OpenVPN_ICE_Client *client, ICE_Server *ice_servers, N_32 ice_servers_length, Address *ice_source, Address *ice_destination, Address *signal_server, Address *OpenVPN_address);
procedure         deinitialize_OpenVPN_ICE_Client (OpenVPN_ICE_Client *client);


#include "client.c"

#endif //OPEN_VPN_ICE_CLIENT_H_INCLUDED
