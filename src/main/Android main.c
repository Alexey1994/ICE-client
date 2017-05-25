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


char* convert_Java_String_to_c_char_array(JNIEnv *env, jstring string)
{
    int i;

    int   length     = (*env)->GetStringLength(env, string);
    char *char_array = new_array(char, length+1);
    char *buffer     = (*env)->GetStringChars(env, string, 0);

    for(i=0; i<length; ++i)
        char_array[i] = buffer[i*2];

    char_array[i] = '\0';

    return char_array;
}


JNIEXPORT jstring JNICALL
Java_com_example_abar0217_stun_MainActivity_stringFromJNI(JNIEnv *env, jobject object, jarray arguments)
{
    set_log_function( print_Android_message );

    int   i;
    char *out;

    int    arguments_length = (*env)->GetArrayLength(env, arguments);
    char **c_arguments = new_array(char*, arguments_length+1);

    c_arguments[0] = "ICE";

    for(i=0; i<arguments_length; ++i)
    {
        jstring   argument = (*env)->GetObjectArrayElement(env, arguments, i);
        c_arguments[i+1] = convert_Java_String_to_c_char_array(env, argument);
    }


    char  buf[200];

    int status = main(arguments_length + 1, c_arguments);

    sprintf(buf, "\n\nProcess returned %d (0x%08x)", status, status);
    print_log(buf);

    if(Android_log_stream)
    	out = (*env)->NewStringUTF(env, Android_log_stream->begin);
    else
    	out = (*env)->NewStringUTF(env, "");

    for(i=0; i<arguments_length; ++i)
        free(c_arguments[i+1]);
    free(c_arguments);

    clear_string(Android_log_stream);

    return out;
}