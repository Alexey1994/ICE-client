#ifndef OPENVPN_ICE_H_INCLUDED
#define OPENVPN_ICE_H_INCLUDED

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    uint8_t  length;
    uint8_t  family;
    uint16_t port;

    union
    {
        uint32_t ip_v4;
        uint32_t ip_v6_flow_info;
    };

    uint8_t ip_v6[16];

    union
    {
        uint32_t ip_v6_scope_id;

        union
        {
            struct
            {
                uint32_t zone: 28;
                uint32_t level: 4;
            };

            uint32_t value;
        };
    };
}
address_t; //sockaddr_in and sockaddr_in6 compatible

typedef enum
{
    STUN_SERVER = 1,
    TURN_SERVER = 2
}
ice_server_types;

typedef struct
{
    uint8_t   type;
    uint8_t   username[32];
    uint8_t   password[32];
    address_t address;
}
ice_server_t;

typedef struct
{
    struct{
        int         socket;
        address_t  *source;
        address_t  *destination;
        uint8_t    *attributes;
        bool        server_is_running;

        bool (*on_request) (address_t *client_address, uint8_t *data, uint32_t data_length, uint8_t *attributes);
    };

    address_t   *input;
    address_t    output;
    bool         output_initialized;

    struct{
        ice_server_t     *servers;
        uint32_t          servers_length;
        address_t        *source2;
        address_t        *destination2;
        address_t        *signal_server;

        struct{
            struct{
                int      socket2;
                uint32_t maximum_waiting_time;
                uint32_t current_time;
            };

            address_t *server_address;
        };

        uint8_t  *local_candidates;
        uint32_t  length_local_candidates;

        uint8_t  *remote_candidates;
        uint32_t  length_remote_candidates;

        struct{
            uint32_t length;
            uint32_t size_of_element;
            int32_t  (*compare_elements) (uint8_t *element1, uint8_t *element2);
            void     (*delete_element)   (uint8_t *element);

            struct{
                uint32_t  reserve;
                uint32_t  begin_index;
                uint32_t  end_index;
                bool      end_of_buffer;
                uint8_t  *data;
            };
        };

        uint8_t  *current_candidate;
    };
}
ice_client_storage_t;

bool initialize_OpenVPN_ICE_Client (ice_client_storage_t *client, ice_server_t *ice_servers, uint32_t ice_servers_length, address_t *ice_source, address_t *ice_destination, address_t *signal_server, address_t *OpenVPN_address);
void deinitialize_OpenVPN_ICE_Client (ice_client_storage_t *client);

typedef struct
{
    ice_server_t *ice_servers;
    uint32_t      ice_servers_length;
    address_t    *signal_server_address;
    address_t    *server_address;
    address_t    *OpenVPN_address;
    bool          is_running;

    struct{
        uint32_t  reserve;
        uint32_t  begin_index;
        uint32_t  end_index;
        bool      end_of_buffer;
        uint8_t  *data;
    };

    struct{
        uint8_t *source;
        void (*write_byte)(uint8_t *source, uint8_t byte);
    };
}
ice_server_storage_t;

bool initialize_OpenVPN_ICE_Server (ice_server_storage_t *server, address_t *ice_local_address, ice_server_t *ice_servers, uint32_t ice_servers_length, address_t *signal_server_address, address_t *OpenVPN_address);
void deinitialize_OpenVPN_ICE_Server (ice_server_storage_t *server);

#endif //OPENVPN_ICE_H_INCLUDED