#include "../../../cryptography/cryptography.h"


void read_MESSAGE_INTEGRITY_attribute(STUN_Attributes *attributes, Byte *attribute, int length)
{

}


void add_MESSAGE_INTEGRITY(String *message, Character *username, Character *realm, Character *password)
{
    N_32   i;
	Byte   HMAC_SHA1_hash[20];
	Byte   MD5_key[16];
	Buffer key;
	Output key_output;
	Buffer data;
	Output data_output;


	initialize_buffer(&key, 1);
	initialize_buffer_output(&key, &key_output);

    write_null_terminated_byte_array(&key_output, username);
    write_byte(&key_output, ':');

    write_null_terminated_byte_array(&key_output, realm);
    write_byte(&key_output, ':');

    write_null_terminated_byte_array(&key_output, password); //SASLprep(password), rfc5389

    calculate_MD5_hash(&key, MD5_key);

    deinitialize_buffer(&key);
    initialize_buffer(&key, 1);

	for(i=0; i<16; ++i)
        write_in_buffer(&key, MD5_key[i]);


    initialize_buffer(&data, 1);
    initialize_buffer_output(&data, &data_output);

    set_STUN_content_length(message->begin, message->length);

    for(i=0; i<message->length; ++i)
        write_byte(&data_output, message->begin[i]);

    while(message->length % 64)
        write_byte(&data_output, 0);


    calculate_HMAC_SHA_1_hash(&data, &key, HMAC_SHA1_hash);

	add_STUN_atribute_head(message, MESSAGE_INTEGRITY_STUN_ATTRIBUTE, 20);
	write_byte_array_to_string(message, HMAC_SHA1_hash, 20);

	deinitialize_buffer(&key);
	deinitialize_output(&key);
	deinitialize_buffer(&data);
    deinitialize_output(&data);
}
