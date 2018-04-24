//#include "system/Linux.h"
//#include "system/Android.h"
#include "system/Windows.h"

#include "system/interfaces/debug/debug.h"
Output stderr;
Output stdout;
#include "network/ICE/ICE.h"


#if (defined(__linux__) && !defined(__ANDROID__)) || defined(__WIN32__)


///////////////////////////////OpenVPN proxy////////////////////////////////////


function Boolean test_OpenVPN_proxy_server()
{
    Proxy_Server server;
    ICE_Server   servers[1];

    servers[0].type = STUN_SERVER;

    if(!initialize_address(&servers[0].address, "192.168.99.100", "3478", UDP_PROTOCOL))
        goto error;

    if(!initialize_proxy_server(&server, &servers, 1, "1234567890123456", "1234567890123455"))
        goto error;

    for(;;);

    return 1;

error:
    printf("error\n");
    return 0;
}


///////////////////////////////test STUN connection/////////////////////////////


function Boolean test_STUN_connection(Byte *STUN_host, Byte *STUN_port)
{
    Address        mapped_address;
    UDP_Connection connection;

    if(!initialize_UDP_Connection(&connection, STUN_host, STUN_port, 10000))
        goto error;

    if(!get_mapped_address(&connection, &mapped_address))
        goto error;

    if(mapped_address.family == 1)
    {
        printf("mapped address: %d.%d.%d.%d:%d\n",
            (mapped_address.ip_v4>>24) & 0xff,
            (mapped_address.ip_v4>>16) & 0xff,
            (mapped_address.ip_v4>>8) & 0xff,
            (mapped_address.ip_v4) & 0xff,
            mapped_address.port
        );
    }
    else
    {
        printf("mapped address: %02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%d\n",
            mapped_address.ip_v6[0],
            mapped_address.ip_v6[1],
            mapped_address.ip_v6[2],
            mapped_address.ip_v6[3],
            mapped_address.ip_v6[4],
            mapped_address.ip_v6[5],
            mapped_address.ip_v6[6],
            mapped_address.ip_v6[7],
            mapped_address.ip_v6[8],
            mapped_address.ip_v6[9],
            mapped_address.ip_v6[10],
            mapped_address.ip_v6[11],
            mapped_address.ip_v6[12],
            mapped_address.ip_v6[13],
            mapped_address.ip_v6[14],
            mapped_address.ip_v6[15],
            mapped_address.port
        );
    }

    return 1;

error:
    return 0;
}


///////////////////////////////test TURN connection/////////////////////////////


Address *client_1_address = 0;
Address *client_2_address = 0;


TCP_Connection signal_server_connection;


function Boolean send_TURN_address(Byte *client_id, Address *my_address)
{
    Buffer       json_buffer;
    Output       json_output;
    Buffer       http_buffer;
    Output       http_output;
    Base64_Coder encoder;
    N_32         i;

    initialize_buffer(&json_buffer, 1);
    initialize_buffer_output(&json_buffer, &json_output);
    initialize_buffer(&http_buffer, 1024);
    initialize_buffer_output(&http_buffer, &http_output);

    write_null_terminated_byte_array(&json_output, "{\"address\":\"");
        begin_base64_encoder(&encoder, &json_output, &write_byte);
            for(i=0; i<sizeof(Address); ++i)
                encode_base64(&encoder, ((Byte*)my_address)[i]);
        end_base64_encoder(&encoder);
    write_null_terminated_byte_array(&json_output, "\",");

    write_null_terminated_byte_array(&json_output, "\"id\":\"");
        begin_base64_encoder(&encoder, &json_output, &write_byte);
            for(i=0; i<16; ++i)
                encode_base64(&encoder, client_id[i]);
        end_base64_encoder(&encoder);
    write_null_terminated_byte_array(&json_output, "\"}");

    write_null_terminated_byte_array(&http_output, "POST /registrate HTTP/1.1\r\n");
    write_null_terminated_byte_array(&http_output, "Host: ");
        write_null_terminated_byte_array(&http_output, "127.0.0.1:8080");
        write_null_terminated_byte_array(&http_output, "\r\n");
    write_null_terminated_byte_array(&http_output, "User-Agent: Mozilla/5.0\r\n");
    write_null_terminated_byte_array(&http_output, "Content-Type: application/json\r\n");
    write_null_terminated_byte_array(&http_output, "Content-Length: ");
        write_N_32(&http_output, buffer_length(&json_buffer));
        write_null_terminated_byte_array(&http_output, "\r\n");
    write_null_terminated_byte_array(&http_output, "Connection: Keep-Alive\r\n\r\n");

    write_byte_array(&http_output, json_buffer.data, buffer_length(&json_buffer));

    if(!write_in_TCP_Connection(&signal_server_connection, http_buffer.data, buffer_length(&http_buffer)))
        goto error;

    if(!read_from_TCP_Connection(&signal_server_connection, http_buffer.data, http_buffer.reserve))
        goto error;

    deinitialize_buffer(&http_buffer);
    deinitialize_buffer(&json_buffer);

    return 1;

error:
    deinitialize_buffer(&http_buffer);
    deinitialize_buffer(&json_buffer);
    return 0;
}


function Boolean get_TURN_address(Byte *client_id, Address *client_address)
{
    Buffer        json_buffer;
    Output        json_output;
    Buffer        http_buffer;
    Output        http_output;
    Base64_Coder  encoder;
    Base64_Coder  decoder;
    Static_Buffer address_buffer;
    N_32          i;
    N_32          j;

    initialize_buffer(&json_buffer, 1);
    initialize_buffer_output(&json_buffer, &json_output);
    initialize_buffer(&http_buffer, 1024);
    initialize_buffer_output(&http_buffer, &http_output);

    write_null_terminated_byte_array(&json_output, "{\"id\":\"");
        begin_base64_encoder(&encoder, &json_output, &write_byte);
            for(i=0; i<16; ++i)
                encode_base64(&encoder, client_id[i]);
        end_base64_encoder(&encoder);
    write_null_terminated_byte_array(&json_output, "\"}");

    write_null_terminated_byte_array(&http_output, "POST /address HTTP/1.1\r\n");
    write_null_terminated_byte_array(&http_output, "Host: ");
        write_null_terminated_byte_array(&http_output, "127.0.0.1:8080");
        write_null_terminated_byte_array(&http_output, "\r\n");
    write_null_terminated_byte_array(&http_output, "User-Agent: Mozilla/5.0\r\n");
    write_null_terminated_byte_array(&http_output, "Content-Type: application/json\r\n");
    write_null_terminated_byte_array(&http_output, "Content-Length: ");
        write_N_32(&http_output, buffer_length(&json_buffer));
        write_null_terminated_byte_array(&http_output, "\r\n");
    write_null_terminated_byte_array(&http_output, "Connection: Keep-Alive\r\n\r\n");

    write_byte_array(&http_output, json_buffer.data, buffer_length(&json_buffer));

    if(!write_in_TCP_Connection(&signal_server_connection, http_buffer.data, buffer_length(&http_buffer)))
        goto error;

    if(!read_from_TCP_Connection(&signal_server_connection, http_buffer.data, http_buffer.reserve))
        goto error;

    for(i=0; http_buffer.data[i] != '\r' || http_buffer.data[i+1] != '\n' || http_buffer.data[i+2] != '\r' || http_buffer.data[i+3] != '\n'; ++i);
    i += 4;

    initialize_static_buffer(&address_buffer, client_address, sizeof(Address));

    begin_base64_decoder(&decoder, &address_buffer, &write_in_static_buffer);
        for(j=0; j<32; ++j)
            decode_base64(&decoder, http_buffer.data[i + j]);
    end_base64_decoder(&decoder);

    deinitialize_buffer(&http_buffer);
    deinitialize_buffer(&json_buffer);

    return 1;

error:
    deinitialize_buffer(&http_buffer);
    deinitialize_buffer(&json_buffer);
    return 0;
}


function Boolean test_TURN_receive(Byte *TURN_host, Byte *TURN_port, Byte *username, Byte *password, Byte *local_id, Byte *remote_id)
{
    TURN_Channel channel;
    Buffer       response;
    N_32         i;

    initialize_buffer(&response, 1);

    if(!initialize_TURN_Channel(&channel, TURN_host, TURN_port, username, password, local_id, &send_TURN_address, remote_id, &get_TURN_address))
        goto error;

    if(!receive_data_from_TURN_client(&channel, &response))
        goto error;

    printf("received %d bytes:", buffer_length(&response));

    for(i=response.begin_index; i<response.end_index; ++i)
        printf("%c", response.data[i]);

    printf("\n");

    deinitialize_buffer(&response);

    return 1;

error:
    deinitialize_buffer(&response);
    return 0;
}


function Boolean test_TURN_transmit(Byte *TURN_host, Byte *TURN_port, Byte *username, Byte *password, Byte *local_id, Byte *remote_id, Byte *message)
{
    TURN_Channel channel;
    Buffer       request;
    N_32         i;

    initialize_buffer(&request, 1);

    for(; *message; ++message)
        write_in_buffer(&request, *message);

    if(!initialize_TURN_Channel(&channel, TURN_host, TURN_port, username, password, local_id, &send_address, remote_id, &get_address))
        goto error;

    if(!transmit_data_to_TURN_client(&channel, &request))
        goto error;

    deinitialize_buffer(&request);

    return 1;

error:
    deinitialize_buffer(&request);
    return 0;
}


////////////////////////////////////////////////////////////////////////////////


internal function test_ICE_transmit()
{
    ICE_Channel channel;
    ICE_Server  servers[1];
    Address     source;
    Address     destination;
    Address     signal_server;

    servers[0].type = TURN_SERVER;
    strcpy(servers[0].username, "");
    strcpy(servers[0].password, "");

    if(!initialize_address(&servers[0].address, "10.101.43.135", "3478", UDP_PROTOCOL))
        goto error;

    if(!initialize_address(&source, "127.0.0.1", "8002", UDP_PROTOCOL))
        goto error;

    if(!initialize_address(&destination, "127.0.0.1", "8001", UDP_PROTOCOL))
        goto error;

    if(!initialize_address(&signal_server, "127.0.0.1", "8080", TCP_PROTOCOL))
        goto error;

    if(!initialize_ICE_Channel(&channel, servers, 1, &source, &destination, &signal_server))
        goto error;

    if(!transmit_data_using_ICE_Channel(&channel, "Hi", 2))
        goto error;

    deinitialize_ICE_Channel(&channel);

    return 1;

error:
    printf("error transmit\n");
    return 0;
}


internal function Boolean test_ICE_receive()
{
    ICE_Channel channel;
    ICE_Server  servers[1];
    Address     source;
    Address     destination;
    Address     signal_server;

    servers[0].type = TURN_SERVER;
    strcpy(servers[0].username, "");
    strcpy(servers[0].password, "");

    if(!initialize_address(&servers[0].address, "10.101.43.135", "3478", UDP_PROTOCOL))
        goto error;

    if(!initialize_address(&source, "127.0.0.1", "8001", UDP_PROTOCOL))
        goto error;

    if(!initialize_address(&destination, "127.0.0.1", "8002", UDP_PROTOCOL))
        goto error;

    if(!initialize_address(&signal_server, "127.0.0.1", "8080", TCP_PROTOCOL))
        goto error;

    if(!initialize_ICE_Channel(&channel, servers, 1, &source, &destination, &signal_server))
        goto error;

    N_32 buffer_length;
    Byte buffer[200];

    buffer_length = 200;

    if(!receive_data_using_ICE_Channel(&channel, buffer, &buffer_length))
        goto error;

    N_32 i;
    printf("receive: ");
    for(i=0; i<buffer_length; ++i)
        printf("%c", buffer[i]);
    printf("\n");

    deinitialize_ICE_Channel(&channel);

    return 1;

error:
    printf("error receive\n");
    return 0;
}


internal procedure test_ICE_transmit_receive()
{
    run_thread(&test_ICE_receive, 0);
    sleep_thread(1000);
    run_thread(&test_ICE_transmit, 0);
    sleep_thread(2000);
}


N_32 main (Z_32 arguments_length, char *arguments[])
{
/*
    Byte signal_server_host[40];
    Byte signal_server_port[6];

    Byte turn_host[40];
    Byte turn_port[6];

    Byte username[200];
    Byte password[200];

    Byte local_id[16];
    Byte remote_id[16];

    Byte message[1500];

    Boolean is_transmit;

    printf("Signal server host: ");
    scanf("%s", signal_server_host);
    printf("Signal server port: ");
    scanf("%s", signal_server_port);

    printf("TURN server host: ");
    scanf("%s", turn_host);
    printf("TURN server port: ");
    scanf("%s", turn_port);

    printf("TURN username: ");
    scanf("%s", username);
    printf("TURN password: ");
    scanf("%s", password);

    printf("local ID: ");
    scanf("%s", local_id);
    printf("remote ID: ");
    scanf("%s", remote_id);

    if(!initialize_TCP_Connection(&signal_server_connection, signal_server_host, signal_server_port, 10000))
        goto error;

    for(;;)
    {
        printf("0 - receive, 1 - transmit: ");
        scanf("%d", &is_transmit);

        if(is_transmit)
        {
            printf("message: ");
            scanf("%s", message);

            if(!test_TURN_transmit(turn_host, turn_port, username, password, local_id, remote_id, message))
                printf("error\n");
        }
        else
        {
            if(!test_TURN_receive(turn_host, turn_port, username, password, local_id, remote_id))
                printf("error\n");
        }

        sleep_thread(1000);
    }
*/
    
    //if(!test_STUN_connection("192.168.99.100", "3478"))
    //    goto error;

    test_ICE_transmit_receive();

    return 0;

error:
    printf("error\n");
    return 1;
}

#endif


#ifdef __ANDROID__

procedure initialize_null_terminated_byte_array_from_Java_string (JNIEnv *env, Byte **byte_array, jstring string)
{
    N_32  i;
    N_32  length = (*env)->GetStringLength(env, string);
    Byte *buffer = (*env)->GetStringChars(env, string, 0);

    length = (*env)->GetStringLength(env, string);
    *byte_array = new_array(Byte, length+1);
    buffer = (*env)->GetStringChars(env, string, 0);

    for(i=0; i<length; ++i)
        (*byte_array)[i] = buffer[i*2];

    (*byte_array)[i] = '\0';
}

procedure deinitialize_null_terminated_byte_array(Byte **byte_array)
{
    free_memory(*byte_array);
}

JNIEXPORT jstring JNICALL
Java_com_example_abar0217_stun_MainActivity_getMappedAddress(JNIEnv *env, jobject object, jstring host, jstring port)
{
    Byte           *converted_host;
    Byte           *converted_port;
    Address         mapped_address;
    Byte            address_string[22];
    UDP_Connection  connection;

    initialize_null_terminated_byte_array_from_Java_string(env, &converted_host, host);
    initialize_null_terminated_byte_array_from_Java_string(env, &converted_port, port);

    if(!initialize_UDP_Connection(&connection, converted_host, converted_port, 1000))
        goto error;

    if(!get_mapped_address(&connection, &mapped_address))
        goto error;

    if(mapped_address.family == 1)
    {
        snprintf(address_string, 22, "%d.%d.%d.%d:%d",
            (mapped_address.ip_v4>>24) & 0xff,
            (mapped_address.ip_v4>>16) & 0xff,
            (mapped_address.ip_v4>>8) & 0xff,
            (mapped_address.ip_v4) & 0xff,
            mapped_address.port
        );
    }
    else
    {
        snprintf(address_string, 40, "%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x",
            mapped_address.ip_v6[0],
            mapped_address.ip_v6[1],
            mapped_address.ip_v6[2],
            mapped_address.ip_v6[3],
            mapped_address.ip_v6[4],
            mapped_address.ip_v6[5],
            mapped_address.ip_v6[6],
            mapped_address.ip_v6[7],
            mapped_address.ip_v6[8],
            mapped_address.ip_v6[9],
            mapped_address.ip_v6[10],
            mapped_address.ip_v6[11],
            mapped_address.ip_v6[12],
            mapped_address.ip_v6[13],
            mapped_address.ip_v6[14],
            mapped_address.ip_v6[15]
        );
    }

    deinitialize_null_terminated_byte_array(&converted_host);
    deinitialize_null_terminated_byte_array(&converted_port);

    deinitialize_UDP_Connection(&connection);

    return (*env)->NewStringUTF(env, address_string);

error:
    return (*env)->NewStringUTF(env, "error");
}

#endif
