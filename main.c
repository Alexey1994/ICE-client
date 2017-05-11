#include "extends/extends.c"
#include "system library/system library.c"
#include "data structures/character/character.c"
#include "data structures/string/string.c"
#include "data structures/random access structure/random access structure.c"
#include "data structures/array/array.c"
#include "stream/stream.c"
#include "thread/thread.c"
#include "error/error.c"
#include "logger/logger.c"
#include "network/network connection/network connection.c"
#include "network/network info/network info.c"

#include "network/STUN/attributes/ALTERNATE_SERVER.c"
#include "network/STUN/attributes/CHANGE_REQUEST.c"
#include "network/STUN/attributes/CHANGED_ADDRESS.c"
#include "network/STUN/attributes/ERROR_CODE.c"
#include "network/STUN/attributes/FINGERPRINT.c"
#include "network/STUN/attributes/MAPPED_ADDRESS.c"
#include "network/STUN/attributes/MESSAGE_INTEGRITY.c"
#include "network/STUN/attributes/NONCE.c"
#include "network/STUN/attributes/PASSWORD.c"
#include "network/STUN/attributes/REALM.c"
#include "network/STUN/attributes/REFLECTED_FROM.c"
#include "network/STUN/attributes/RESPONSE_ADDRESS.c"
#include "network/STUN/attributes/SOFTWARE.c"
#include "network/STUN/attributes/SOURCE_ADDRESS.c"
#include "network/STUN/attributes/UNKNOWN_ATTRIBUTES.c"
#include "network/STUN/attributes/USERNAME.c"
#include "network/STUN/attributes/XOR_MAPPED_ADDRESS.c"
#include "network/STUN/STUN.c"
#include "network/STUN/debug.c"


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
    initialize_STUN();

    //get_NAT_type_using_STUN_server("127.0.0.1", 3478);
    //get_NAT_type_using_STUN_server("stun.l.google.com", 19302);

    if(arguments_length < 2)
        get_NAT_type_using_STUN_server("stun.ideasip.com", 3478);
    else if(arguments_length == 2)
        get_NAT_type_using_STUN_server(arguments[1], 3478);
    else
        get_NAT_type_using_STUN_server(arguments[1], atoi(arguments[2]));

    return 0;
}

#endif // __WIN32__
