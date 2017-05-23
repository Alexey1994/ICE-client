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