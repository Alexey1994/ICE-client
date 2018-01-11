#define ENABLE_DEBUG

#include "main/main.h"
#include "cryptography/cryptography.h"

//#include "network/network info/network info.h"
#include "network/STUN/STUN.h"
#include "network/TURN/TURN.h"
#include "network/network server/network server.h"
//#include "network/connection/UDP/UDP.h"
#include "network/connection/TCP/TCP.h"

#define CLIENT_ID 2

#define STUN_HOST "192.168.56.102"
#define STUN_PORT 3478


#define TURN_HOST "192.168.56.102"
#define TURN_PORT 3478

#define SIGNAL_SERVER_HOST "127.0.0.1"
#define SIGNAL_SERVER_PORT 8080


int sender_listener(Byte *data)
{
    printf("message: %s\n", data);
    //printf("listen client\n");
    //printf("listen client %s\n", data);
}


void read_HTTP_data(char *HTTP_data, char *buffer)
{
    for(; *HTTP_data; ++HTTP_data)
    {
        if(*HTTP_data == '\r' && HTTP_data[1] == '\n' && HTTP_data[2] == '\r' && HTTP_data[3] == '\n')
        {
            HTTP_data += 4;
            break;
        }
    }

    strcpy(buffer, HTTP_data);
}


void send_mapped_address(char *host, int port)
{
    TCP_Connection *signal_server = create_TCP(SIGNAL_SERVER_HOST, SIGNAL_SERVER_PORT);
    char            buffer[512];

    snprintf(buffer, 512,
        "GET /reg%d?host=%s&port=%d HTTP/1.1\r\n"
        "Host: %s:%d\r\n"
        "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:55.0) Gecko/20100101 Firefox/55.0\r\n"
        "Accept: text/html\r\n"
        "Connection: close\r\n\r\n",
        CLIENT_ID,

        host,
        port,

        SIGNAL_SERVER_HOST,
        SIGNAL_SERVER_PORT
    );

    write_in_TCP(signal_server, buffer, strlen(buffer));
    memset(buffer, 0, 512);
    read_from_TCP(signal_server, buffer, 512);
}


void get_client_host(char *host)
{
    TCP_Connection *signal_server = create_TCP(SIGNAL_SERVER_HOST, SIGNAL_SERVER_PORT);
    char            buffer[512];
    int             i;

    snprintf(buffer, 512,
        "GET /host%d HTTP/1.1\r\n"
        "Host: %s:%d\r\n"
        "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:55.0) Gecko/20100101 Firefox/55.0\r\n"
        "Accept: text/html\r\n"
        "Connection: close\r\n\r\n",
        CLIENT_ID,
        SIGNAL_SERVER_HOST,
        SIGNAL_SERVER_PORT
    );

    write_in_TCP(signal_server, buffer, strlen(buffer));
    memset(buffer, 0, 512);
    read_from_TCP(signal_server, buffer, 512);

    read_HTTP_data(buffer, host);
}


int get_client_port()
{
    TCP_Connection *signal_server = create_TCP(SIGNAL_SERVER_HOST, SIGNAL_SERVER_PORT);
    char            buffer[512];
    int             i;

    snprintf(buffer, 512,
        "GET /port%d HTTP/1.1\r\n"
        "Host: %s:%d\r\n"
        "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:55.0) Gecko/20100101 Firefox/55.0\r\n"
        "Accept: text/html\r\n"
        "Connection: close\r\n\r\n",
        CLIENT_ID,
        SIGNAL_SERVER_HOST,
        SIGNAL_SERVER_PORT
    );

    write_in_TCP(signal_server, buffer, strlen(buffer));

    memset(buffer, 0, 512);
    read_from_TCP(signal_server, buffer, 512);
    read_HTTP_data(buffer, buffer);

    return atoi(buffer);
}


void test_STUN_connection()
{
    char           buffer[512];
    char           mapped_host[16];
    unsigned short mapped_port;
    char           client_host[512];
    int            client_port;

    get_STUN_mapped_address(STUN_HOST, STUN_PORT, mapped_host, &mapped_port);
    send_mapped_address(mapped_host, mapped_port);
    Server *sender = create_UDP_server(mapped_host, mapped_port, sender_listener, 0);
    printf("listen messages on address: %s:%d\n", mapped_host, mapped_port);

    sleep_thread(5000);
    get_client_host(client_host);
    client_port = get_client_port();

    UDP_Connection *connection = create_UDP(client_host, client_port);
    printf("connect to client address: %s:%d\n", client_host, client_port);

    for(;;)
    {
        snprintf(buffer, 512, "Hello from client %d", CLIENT_ID);

        write_in_UDP(connection, buffer, strlen(buffer));
        sleep_thread(1000);
    }
}


void test_STUN()
{
    char           mapped_host[16];
    unsigned short mapped_port;

    get_STUN_mapped_address(STUN_HOST, STUN_PORT, mapped_host, &mapped_port);
}


void test_TURN_connection()
{
    N_32            xor_relayed_host;
    N_16            xor_relayed_port;
    TURN_Attributes attributes;
    TURN_Attributes attributes2;

    UDP_Connection *connection = create_UDP(TURN_HOST, TURN_PORT);

    memset(&attributes, 0, sizeof(attributes));
    allocate_TURN(connection, TURN_HOST, TURN_PORT, &attributes);

    if(attributes.STUN_attributes.ERROR_CODE == 401) //unathorized
    {
        //print_MD5_hash(attributes.STUN_attributes.NONCE);
        memset(&attributes2, 0, sizeof(attributes2));
        allocate_TURN_with_authentication(connection, TURN_HOST, TURN_PORT, &attributes2, attributes.STUN_attributes.NONCE.data, attributes.STUN_attributes.NONCE.length, "lex", attributes.STUN_attributes.REALM, "1");
    }

    if(attributes.STUN_attributes.REALM)
        free(attributes.STUN_attributes.REALM);

    if(attributes.STUN_attributes.NONCE.data)
        free(attributes.STUN_attributes.NONCE.data);

    destroy_UDP(connection);

    //create_TURN_permission(TURN_HOST, TURN_PORT, &attributes, xor_relayed_host, xor_relayed_port);
    //bind_TURN_channel(TURN_HOST, TURN_PORT);
    //send_TURN_data(TURN_HOST, TURN_PORT);
    //receive_TURN_data(TURN_HOST, TURN_PORT);
}


void print_test_TURN_vectors(STUN_Head *head)
{
    String     message_string;

    message_string.length = head->content_length;
    convert_big_to_little_endian(&message_string.length, 2);
    message_string.length += 20;
    message_string.begin = head;

    print_TURN_response(&message_string);
}


void get_test_vectors()
{
    unsigned char buffer[512];

    Server *sender = create_UDP_server("127.0.0.1", 8081, print_test_TURN_vectors, 0);

    for(;;);
}


void print_hex_character(Byte hex_number)
{
    if(hex_number > 9)
        printf("%c", hex_number - 10 + 'a');
    else
        printf("%c", hex_number + '0');
}


void print_hex_number(Byte number)
{
    if(number > 0x0f)
    {
        print_hex_character(number >> 4);
        print_hex_character(number & 0x0f);
    }
    else
    {
        printf("0");
        print_hex_character(number);
    }
}


void print_MD5_hash(Byte *hash)
{
    N_32 i;

    for(i=0; i<16; ++i)
        print_hex_number(hash[i]);

    printf("\n");
}


void test_MD5_hash()
{
    Buffer data;
    N_32   hash[4];
    N_32   test_vector[] = {0x0cc175b9, 0xc0f1b6a8, 0x31c399e2, 0x69772661};

    convert_big_to_little_endian(&test_vector[0], 4);
    convert_big_to_little_endian(&test_vector[1], 4);
    convert_big_to_little_endian(&test_vector[2], 4);
    convert_big_to_little_endian(&test_vector[3], 4);

    initialize_buffer(&data, 1);
    write_in_buffer(&data, 'a');

    calculate_MD5_hash(&data, hash);
    //print_MD5_hash(hash);

    if(memcmp(hash, test_vector, 16))
        printf("Test 1(MD5) failed\n");
    else
        printf("Test 1(MD5) success\n");
}


void print_SHA_1_hash(Byte *hash)
{
    N_32 i;

    for(i=0; i<20; ++i)
        print_hex_number(hash[i]);

    printf("\n");
}


void test_HMAC_SHA_1_hash()
{
    Buffer data;
    Buffer key;
    Byte   hash[20];
    N_32   test_vector[] = {0x5b0c157d, 0x4e767244, 0x4c410335, 0x61554839, 0xed1fd2d6};

    convert_big_to_little_endian(&test_vector[0], 4);
    convert_big_to_little_endian(&test_vector[1], 4);
    convert_big_to_little_endian(&test_vector[2], 4);
    convert_big_to_little_endian(&test_vector[3], 4);
    convert_big_to_little_endian(&test_vector[4], 4);

    initialize_buffer(&data, 1);
    initialize_buffer(&key, 1);

    write_in_buffer(&data, '1');
    write_in_buffer(&key, '1');

    calculate_HMAC_SHA_1_hash(&data, &key, hash);
    //print_SHA_1_hash(hash);

    printf("Test 1(HMAC SHA-1) ");

    if(memcmp(hash, test_vector, 20))
        printf("failed\n");
    else
        printf("success\n");
}


void test_MESSAGE_INTEGRITY_hash()
{
    N_32   i;
    Buffer data;
    Output data_output;
    Buffer key;
    Output key_output;
    Byte   MD5_hash[16];
    Byte   hash[20];

    initialize_buffer(&data, 1);
    initialize_buffer_output(&data, &data_output);

    initialize_buffer(&key, 1);
    initialize_buffer_output(&key, &key_output);

    //write_null_terminated_byte_array(&key_output, "<U+30DE><U+30C8><U+30EA><U+30C3><U+30AF><U+30B9>:example.org:The<U+00AD>M<U+00AA>tr<U+2168>");
    //calculate_MD5_hash(&key, MD5_hash);

    write_null_terminated_byte_array(&key_output, "VOkJxbRl1RmTxUk/WvJxBt");
    calculate_MD5_hash(&key, MD5_hash);

    initialize_buffer(&key, 1);
    initialize_buffer_output(&key, &key_output);

    for(i=0; i<16; ++i)
        write_byte(&key_output, MD5_hash[i]);

    //write_null_terminated_byte_array(&data_output,
    write_byte_array(
        &data_output,
        /*"\x00\x01\x00\x54\x21\x12\xA4\x42"
        "\xBE\xEB\xD6\xF1\x77\x10\xB6\xA1"
        "\xB6\xA9\x2C\xBD\x00\x06\x00\x0C"
        "\x76\x4F\x61\x4D\x3A\x66\x76\x41"
        "\x73\x00\x00\x00\x00\x24\x00\x04"
        "\x6E\xFF\xFE\xFF\x80\x29\x00\x08"
        "\x00\x00\x00\x00\x00\x01\xE6\xE4"
        "\x80\x54\x00\x04\x31\x00\x00\x00"
        "\x80\x70\x00\x04\x00\x00\x00\x02"*/

        /*
        "\x00\x01\x00\x48"
         "\x21\x12\xa4\x42"
         "\x78\xad\x34\x33\xc6\xad\x72\xc0\x29\xda\x41\x2e"
         "\x00\x06\x00\x12"
           "\xe3\x83\x9e\xe3\x83\x88\xe3\x83\xaa\xe3\x83\x83"
           "\xe3\x82\xaf\xe3\x82\xb9\x00\x00"
         "\x00\x15\x00\x1c"
           "\x66\x2f\x2f\x34\x39\x39\x6b\x39\x35\x34\x64\x36"
           "\x4f\x4c\x33\x34\x6f\x4c\x39\x46\x53\x54\x76\x79"
           "\x36\x34\x73\x41"
         "\x00\x14\x00\x0b"
           "\x65\x78\x61\x6d\x70\x6c\x65\x2e\x6f\x72\x67\x00"
         "\x00\x08\x00\x14"
         //  "\xf6\x70\x24\x65\x6d\xd6\x4a\x3e\x02\xb8\xe0\x71"
         //  "\x2e\x85\xc9\xa2\x8c\xa8\x96\x66"
        //"\x00\x08\x00\x14"*/
//2b 91 f5 99
     "\x01\x01\x00\x48"
     "\x21\x12\xa4\x42"
     "\xb7\xe7\xa7\x01"
     "\xbc\x34\xd6\x86"
     "\xfa\x87\xdf\xae"
     "\x80\x22\x00\x0b"
     "\x74\x65\x73\x74"
     "\x20\x76\x65\x63"
     "\x74\x6f\x72\x20"
     "\x00\x20\x00\x08"
     "\x00\x01\xa1\x47"
     "\xe1\x12\xa6\x43"
     //"\x00\x08\x00\x14"

        "\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
        0x48 - 20 - 4
    );

    //write_in_buffer(&data, '1');
    //write_in_buffer(&key, '1');

    calculate_HMAC_SHA_1_hash(&data, &key, hash);
    print_SHA_1_hash(hash);
}


void test_STUN_authenticate()
{
    char           mapped_host[16];
    unsigned short mapped_port;

    authenticate_on_STUN_server(STUN_HOST, STUN_PORT);
}


int main(int arguments_length, char *arguments[])
{
    initialize_STUN();
    initialize_TURN();
    //get_test_vectors();
    //test_STUN();
    //test_STUN_authenticate();
    //test_TURN_connection();
    test_MESSAGE_INTEGRITY_hash();
    //test_MD5_hash();
    //test_HMAC_SHA_1_hash();

    /*
    char           mapped_host[16];
    unsigned short mapped_port;

    if(arguments_length < 2)
    {
        if (!get_STUN_mapped_address("192.168.56.101", 3478, mapped_host, &mapped_port))
            return 0;
    }
    else if(arguments_length == 2)
    {
        if (!get_STUN_mapped_address(arguments[1], 3478, mapped_host, &mapped_port))
            return 0;
    }
    else
    {
        if (!get_STUN_mapped_address(arguments[1], atoi(arguments[2]), mapped_host, &mapped_port))
            return 0;
    }*/

    return 0;
}
