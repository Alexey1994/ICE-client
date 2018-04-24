#ifdef __WIN32__
# include "Windows/Windows.h"
#endif

#ifdef __linux__
# include "Linux/Linux.h"
#endif

#ifdef __APPLE__
# include "Apple/Apple.h"
#endif


internal function Boolean write_address (Output *output, Address *address)
{
    N_32 i;

    if(address->family == IPv4)
    {
        N_8 *address_data;

        address_data = &address->ip_v4;
        
        //host
        for(i=0; i<3; ++i)
        {
            write_N_32(output, address_data[i]);
            write_byte(output, '.');
        }
        write_N_32(output, address_data[i]);

        //port
        N_16 converted_port;

        converted_port = address->port;
        convert_big_to_little_endian(&converted_port, 2);
        write_byte(output, ':');
        write_N_32(output, converted_port);
    }
    else if(address->family == IPv6)
    {

    }
    else
        goto error;

    return 1;

error:
    return 0;
}