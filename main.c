#include "network/network info/network info.h"
#include "network/TURN/TURN.h"


#ifdef __ANDROID_API__

#include <jni.h>


static String *Android_log_stream = 0;


static void print_Android_message(char *message)
{
	if(!Android_log_stream)
		Android_log_stream = create_string(10);

	concatenate_string_with_char_array(Android_log_stream, message);
}

//jarray
JNIEXPORT jstring JNICALL
Java_com_example_abar0217_stun_MainActivity_stringFromJNI(JNIEnv *env, jobject object, jstring log)
{
    initialize_STUN();
    set_log_function( print_Android_message );

    char           mapped_host[16];
    unsigned short mapped_port;
    char           address[21];


    get_STUN_MAPPED_ADDRESS("stun.stunprotocol.org", 3478, mapped_host, &mapped_port);

    //printf("%s:%d", mapped_host, mapped_port);

    //return (*env)->NewStringUTF(env, address);

    if(Android_log_stream)
    {
        log = (*env)->NewStringUTF(env, Android_log_stream->begin);
        return log;
    }

    snprintf(address, 21, "%s:%d", mapped_host, mapped_port);

    return (*env)->NewStringUTF(env, address);
}

#endif //__ANDROID_API__


#ifdef __WIN32__

int main(int arguments_length, char *arguments[])
{
    char           mapped_host[16];
    unsigned short mapped_port;

    initialize_STUN();
    initialize_TURN();

    //get_NAT_type_using_STUN_server("127.0.0.1", 3478);
    //get_NAT_type_using_STUN_server("stun.l.google.com", 19302);


    if(arguments_length < 2)
        if(!get_STUN_mapped_address("192.168.56.1", 3478, mapped_host, &mapped_port))
            return 0;
    else if(arguments_length == 2)
        if(!get_STUN_mapped_address(arguments[1], 3478, mapped_host, &mapped_port))
            return 0;
    else
        if(!get_STUN_mapped_address(arguments[1], atoi(arguments[2]), mapped_host, &mapped_port))
            return 0;

    //get_STUN_mapped_address("192.168.56.1", 3478, mapped_host, &mapped_port);

    char local_address[16];
    get_IPv4_host_address(local_address);
    printf("local address: %s\n", local_address);
    printf("mapped address: %s:%d\n", mapped_host, mapped_port);

    //TURN_TCP_request("192.168.56.1", 3478);
    //TURN_UDP_request("192.168.56.1", 3478);

    return 0;
}

#endif // __WIN32__
