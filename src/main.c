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

#define STUN_HOST "192.168.56.101"
#define STUN_PORT 3478


#define TURN_HOST "turn1.northeurope.cloudapp.azure.com"
#define TURN_PORT 3478

#define SIGNAL_SERVER_HOST "absdf.herokuapp.com"
#define SIGNAL_SERVER_PORT 80


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

    if(SIGNAL_SERVER_PORT != 80)
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
    else
        snprintf(buffer, 512,
            "GET /host%d HTTP/1.1\r\n"
            "Host: %s\r\n"
            "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:55.0) Gecko/20100101 Firefox/55.0\r\n"
            "Accept: text/html\r\n"
            "Connection: close\r\n\r\n",
            CLIENT_ID,
            SIGNAL_SERVER_HOST
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

    if(SIGNAL_SERVER_PORT != 80)
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
    else
        snprintf(buffer, 512,
            "GET /port%d HTTP/1.1\r\n"
            "Host: %s\r\n"
            "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:55.0) Gecko/20100101 Firefox/55.0\r\n"
            "Accept: text/html\r\n"
            "Connection: close\r\n\r\n",
            CLIENT_ID,
            SIGNAL_SERVER_HOST
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



void test_TURN_receive(Character *username, Character *password)
{
    N_32             xor_relayed_host;
    N_16             xor_relayed_port;
    TURN_Attributes  attributes;
    TURN_Attributes  attributes2;
    TURN_Attributes  attributes3;
    TURN_Attributes  attributes4;
    Buffer           long_term_key;
    Buffer           short_term_key;
    Output           short_term_key_output;
    UDP_Connection  *connection;

    char           client_host[512];
    int            client_port;

    char           *my_host;
    N_16            my_port;

    char           *my_mapped_host;
    N_16            my_mapped_port;

    connection = create_UDP(TURN_HOST, TURN_PORT);

    initialize_buffer(&long_term_key, 1);
    initialize_buffer(&short_term_key, 1);

    initialize_buffer_output(&short_term_key, &short_term_key_output);
    write_null_terminated_byte_array(&short_term_key_output, password);
    deinitialize_output(&short_term_key_output);

    memset(&attributes, 0, sizeof(attributes));
    allocate_TURN(connection, &attributes);

    if(attributes.STUN_attributes.ERROR_CODE == 401)
    {
        memset(&attributes2, 0, sizeof(attributes2));
        initialize_long_authentication_key(
            &long_term_key,
            username,
            attributes.STUN_attributes.REALM,
            password
        );

        allocate_TURN_with_authentication(
            connection,
            &attributes2,
            attributes.STUN_attributes.NONCE.data,
            attributes.STUN_attributes.NONCE.length,
            username,
            attributes.STUN_attributes.REALM,
            &long_term_key
        );

        my_host = attributes2.XOR_RELAYED_ADDRESS.host;
        my_port = attributes2.XOR_RELAYED_ADDRESS.port;

        my_mapped_host = attributes2.STUN_attributes.XOR_MAPPED_ADDRESS.host;
        my_mapped_port = attributes2.STUN_attributes.XOR_MAPPED_ADDRESS.port;
    }
    else
    {
        my_host = attributes.XOR_RELAYED_ADDRESS.host;
        my_port = attributes.XOR_RELAYED_ADDRESS.port;

        my_mapped_host = attributes.STUN_attributes.XOR_MAPPED_ADDRESS.host;
        my_mapped_port = attributes.STUN_attributes.XOR_MAPPED_ADDRESS.port;
    }

    send_mapped_address(my_host, my_port);

    printf("my address: %s:%d\n", my_host, my_port);
    printf("my mapped address %s:%d\n", my_mapped_host, my_mapped_port);

        //Server *sender = create_UDP_server(my_host, my_port, sender_listener, 0);

    sleep_thread(1000);
    get_client_host(client_host);
    client_port = get_client_port();

    //UDP_Connection *connection = create_UDP(client_host, client_port);
    printf("connect to client address: %s:%d\n", client_host, client_port);

    memset(&attributes3, 0, sizeof(attributes3));
    create_TURN_permission(connection, &attributes3, client_host, client_port);

    for(;;)
    {
        Byte buf[128];
        read_from_UDP(connection, buf, 128);
        printf("%d %d %d %d %d %d %d %d", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7]);
    }
}


void test_TURN_transmit(Character *username, Character *password)
{
    N_32             xor_relayed_host;
    N_16             xor_relayed_port;
    TURN_Attributes  attributes;
    TURN_Attributes  attributes2;
    TURN_Attributes  attributes3;
    TURN_Attributes  attributes4;
    Buffer           long_term_key;
    Buffer           short_term_key;
    Output           short_term_key_output;
    UDP_Connection  *connection;

    char           client_host[512];
    int            client_port;

    char           *my_host;
    N_16            my_port;

    char           *my_mapped_host;
    N_16            my_mapped_port;

    connection = create_UDP(TURN_HOST, TURN_PORT);

    initialize_buffer(&long_term_key, 1);
    initialize_buffer(&short_term_key, 1);

    initialize_buffer_output(&short_term_key, &short_term_key_output);
    write_null_terminated_byte_array(&short_term_key_output, password);
    deinitialize_output(&short_term_key_output);

    memset(&attributes, 0, sizeof(attributes));
    allocate_TURN(connection, &attributes);

    if(attributes.STUN_attributes.ERROR_CODE == 401)
    {
        memset(&attributes2, 0, sizeof(attributes2));
        initialize_long_authentication_key(
            &long_term_key,
            username,
            attributes.STUN_attributes.REALM,
            password
        );

        allocate_TURN_with_authentication(
            connection,
            &attributes2,
            attributes.STUN_attributes.NONCE.data,
            attributes.STUN_attributes.NONCE.length,
            username,
            attributes.STUN_attributes.REALM,
            &long_term_key
        );

        my_host = attributes2.XOR_RELAYED_ADDRESS.host;
        my_port = attributes2.XOR_RELAYED_ADDRESS.port;

        my_mapped_host = attributes2.STUN_attributes.XOR_MAPPED_ADDRESS.host;
        my_mapped_port = attributes2.STUN_attributes.XOR_MAPPED_ADDRESS.port;
    }
    else
    {
        my_host = attributes.XOR_RELAYED_ADDRESS.host;
        my_port = attributes.XOR_RELAYED_ADDRESS.port;

        my_mapped_host = attributes.STUN_attributes.XOR_MAPPED_ADDRESS.host;
        my_mapped_port = attributes.STUN_attributes.XOR_MAPPED_ADDRESS.port;
    }

    send_mapped_address(my_host, my_port);

    printf("my address: %s:%d\n", my_host, my_port);
    printf("my mapped address %s:%d\n", my_mapped_host, my_mapped_port);

    sleep_thread(1000);
    get_client_host(client_host);
    client_port = get_client_port();

    printf("connect to client address: %s:%d\n", client_host, client_port);


    memset(&attributes3, 0, sizeof(attributes3));
    create_TURN_permission(connection, &attributes3, client_host, client_port);
    //create_TURN_permission(connection, &attributes3, my_host, my_port);

/*
    memset(&attributes3, 0, sizeof(attributes3));
    create_TURN_permission_with_authentication(
        connection,
        &attributes3,
        attributes.STUN_attributes.NONCE.data,
        attributes.STUN_attributes.NONCE.length,
        username,
        attributes.STUN_attributes.REALM,
        &long_term_key,
        my_host,//client_host,//"127.0.0.1",
        my_port//client_port//3478
    );*/

    memset(&attributes4, 0, sizeof(attributes4));
    //send_TURN_data(connection, &attributes4, my_host, my_port);
    send_TURN_data(connection, &attributes4, client_host, client_port);

    for(;;);
}


void test_TURN_connection(Character *username, Character *password)
{
    if(CLIENT_ID == 1)
        test_TURN_transmit("lex", "1");
    else
        test_TURN_receive("lex", "1");
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


procedure initialize_HMAC_SHA_1_test(Buffer *data, Output *data_output, Buffer *key, Output *key_output)
{
    initialize_buffer(data, 1);
    initialize_buffer_output(data, data_output);

    initialize_buffer(key, 1);
    initialize_buffer_output(key, key_output);
}


void test_HMAC_SHA_1_hash_item(N_32 *test_vector, N_32 test_number, Buffer *key, Buffer *data)
{
    Byte hash[20];

    calculate_HMAC_SHA_1_hash(data, key, hash);

    printf("Test %d (HMAC SHA-1) ", test_number);

    if(memcmp(hash, test_vector, 20))
    {
        printf("failed\n");
        print_SHA_1_hash(test_vector);
        print_SHA_1_hash(hash);
        printf("\n");
    }
    else
        printf("success\n");
}


void test_HMAC_SHA_1_hash()
{
    N_32   i;
    N_32   j;
    Buffer data;
    Output data_output;
    Buffer key;
    Output key_output;
    Byte   hash[20];
    N_32   test_vectors[7][5] = {
        {0xb6173186, 0x55057264, 0xe28bc0b6, 0xfb378c8e, 0xf146be00},
        {0xeffcdf6a, 0xe5eb2fa2, 0xd27416d5, 0xf184df9c, 0x259a7c79},
        {0x4c9007f4, 0x026250c6, 0xbc8414f9, 0xbf50c86c, 0x2d7235da},
        {0x4c1a0342, 0x4b55e07f, 0xe7f27be1, 0xd58bb932, 0x4a9a5a04},
        {0xaa4ae5e1, 0x5272d00e, 0x95705637, 0xce8a3b55, 0xed402112},
        {0xe8e99d0f, 0x45237d78, 0x6d6bbaa7, 0x965c7808, 0xbbff1a91}
    };

    for(i=0; i<7; ++i)
        for(j=0; j<5; ++j)
            convert_big_to_little_endian(&test_vectors[i][j], 4);

    initialize_HMAC_SHA_1_test(&data, &data_output, &key, &key_output);

    write_null_terminated_byte_array(&data_output, "Hi There");

    for(i=0; i<20; ++i)
        write_in_buffer(&key, 0x0b);

    test_HMAC_SHA_1_hash_item(test_vectors[0], 1, &key, &data);


    initialize_HMAC_SHA_1_test(&data, &data_output, &key, &key_output);

    write_null_terminated_byte_array(&data_output, "what do ya want for nothing?");
    write_null_terminated_byte_array(&key_output, "Jefe");

    test_HMAC_SHA_1_hash_item(test_vectors[1], 2, &key, &data);


    initialize_HMAC_SHA_1_test(&data, &data_output, &key, &key_output);

    for(i=0; i<50; ++i)
        write_byte(&data_output, 0xcd);

    for(i=0; i<25; ++i)
        write_byte(&key_output, i+1);

    test_HMAC_SHA_1_hash_item(test_vectors[2], 3, &key, &data);


    initialize_HMAC_SHA_1_test(&data, &data_output, &key, &key_output);

    write_null_terminated_byte_array(&data_output, "Test With Truncation");

    for(i=0; i<20; ++i)
        write_byte(&key_output, 0x0c);

    test_HMAC_SHA_1_hash_item(test_vectors[3], 4, &key, &data);


    initialize_HMAC_SHA_1_test(&data, &data_output, &key, &key_output);

    write_null_terminated_byte_array(&data_output, "Test Using Larger Than Block-Size Key - Hash Key First");

    for(i=0; i<80; ++i)
        write_byte(&key_output, 0xaa);

    test_HMAC_SHA_1_hash_item(test_vectors[4], 5, &key, &data);


    initialize_HMAC_SHA_1_test(&data, &data_output, &key, &key_output);

    write_null_terminated_byte_array(&data_output, "Test Using Larger Than Block-Size Key and Larger Than One Block-Size Data");

    for(i=0; i<80; ++i)
        write_byte(&key_output, 0xaa);

    test_HMAC_SHA_1_hash_item(test_vectors[5], 6, &key, &data);
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

    test_TURN_connection("lex", "1");

    //test_MESSAGE_INTEGRITY_hash();
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
