#if defined(__WIN32__)
# include "system/Windows.h"
#elif defined(__ANDROID__)
# include "system/Android.h"
#elif defined(__linux__)
# include "system/Linux.h"
#elif defined(__APPLE__)
# include "system/Apple.h"
#endif

//#include <stdio.h>

#include "system/interfaces/debug/debug.h"
Output debug_output;

#include "network/ICE/ICE.h"
#include "network/ICE/OpenVPN/client/client.h"
#include "network/ICE/OpenVPN/server/server.h"


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

    if(!initialize_address(&servers[0].address, "10.200.200.24", "3478", UDP_PROTOCOL))
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

    if(!initialize_address(&servers[0].address, "127.0.0.1", "3478", UDP_PROTOCOL))
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
    //run_thread(&test_ICE_receive, 0);
    //sleep_thread(1000);
    run_thread(&test_ICE_transmit, 0);
    sleep_thread(2000);
}


int main(){
    initialize_debug_output(&debug_output);
    test_ICE_transmit_receive();

    return 0;
}


/*
#if (defined(__linux__) && !defined(__ANDROID__)) || defined(__WIN32__)


N_32 main()
{
    OpenVPN_ICE_Client client;
    ICE_Server         servers[1];
    Address            source;
    Address            destination;
    Address            signal_server;
    Address            OpenVPN_address;

    servers[0].type = STUN_SERVER;

    if(!initialize_address(&servers[0].address, "192.168.99.100", "3478", UDP_PROTOCOL))
        goto error;

    if(!initialize_address(&source, "127.0.0.1", "8002", UDP_PROTOCOL))
        goto error;

    if(!initialize_address(&destination, "127.0.0.1", "8001", UDP_PROTOCOL))
        goto error;

    if(!initialize_address(&signal_server, "192.168.99.100", "8080", TCP_PROTOCOL))
        goto error;

    if(!initialize_address(&OpenVPN_address, "127.0.0.1", "8001", UDP_PROTOCOL))
        goto error;

    if(!initialize_OpenVPN_ICE_Client(&client, &servers, 1, &source, &destination, &signal_server, &OpenVPN_address))
        goto error;

    for(;;)
        sleep_thread(100);

    deinitialize_OpenVPN_ICE_Client(&client);

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


ICE_Channel channel;
int channel_initialized = 0;


JNIEXPORT jstring JNICALL
Java_com_example_abar0217_stun_MainActivity_sendData(JNIEnv *env, jobject object, jstring data)
{
    Byte *converted_data;

    initialize_null_terminated_byte_array_from_Java_string(env, &converted_data, data);

    ICE_Server  servers[1];

    servers[0].type = TURN_SERVER;
    strcpy(servers[0].host, "192.168.99.100");
    strcpy(servers[0].port, "3478");
    strcpy(servers[0].username, "lex");
    strcpy(servers[0].password, "1");

    if(!channel_initialized && !initialize_ICE_Channel(&channel, servers, 1, "1234567890123456", "1234567890123455", "192.168.99.100", "8080"))
        goto error;

        channel_initialized = 1;

        if(!transmit_data_using_ICE_Channel(&channel, converted_data, strlen(converted_data)))
            goto error;

    //deinitialize_ICE_Channel(&channel);

    deinitialize_null_terminated_byte_array(&converted_data);

    return (*env)->NewStringUTF(env, "success");

    error:
        return (*env)->NewStringUTF(env, "error");
}


JNIEXPORT jstring JNICALL
Java_com_example_abar0217_stun_MainActivity_getData(JNIEnv *env, jobject object)
{
    Byte  message[1500] = {};
    int   message_length;

    ICE_Server  servers[1];

    servers[0].type = TURN_SERVER;
    strcpy(servers[0].host, "192.168.99.100");
    strcpy(servers[0].port, "3478");
    strcpy(servers[0].username, "lex");
    strcpy(servers[0].password, "1");

    if(!channel_initialized && !initialize_ICE_Channel(&channel, servers, 1, "1234567890123456", "1234567890123455", "192.168.99.100", "8080"))
        goto error;

    channel_initialized = 1;

    message_length = 1500;

    if(!receive_data_using_ICE_Channel(&channel, message, &message_length))
        goto error;

    //deinitialize_ICE_Channel(&channel);

    return (*env)->NewStringUTF(env, message);

    error:
    return (*env)->NewStringUTF(env, "error");
}


#endif
*/
