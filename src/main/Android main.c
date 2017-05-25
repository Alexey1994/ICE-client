#include <jni.h>
#include "../data structures/string/string.h"
#include "../logger/logger.h"


static String *Android_log_stream = 0;


static void print_Android_message(char *message)
{
	if(!Android_log_stream)
		Android_log_stream = create_string(10);

	concatenate_string_with_char_array(Android_log_stream, message);
}


JNIEXPORT jstring JNICALL
Java_com_example_abar0217_stun_MainActivity_stringFromJNI(JNIEnv *env, jobject object, jstring log)
{
    char *arguments[] = {"ICE"};
    char  buf[200];

    set_log_function( print_Android_message );

    int status = main(1, arguments);

    snprintf(buf, 200, "\n\nProcess returned %d (0x%08x)", status, status);
    print_log(buf);

    if(Android_log_stream)
        return (*env)->NewStringUTF(env, Android_log_stream->begin);

    return (*env)->NewStringUTF(env, "");
}