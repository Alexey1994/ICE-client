#ifndef HTTP_H_INCLUDED
#define HTTP_H_INCLUDED


#include "../../input/input.h"
#include "../../output/output.h"
//#include "../../data structures/prefix tree/prefix tree.h"


typedef enum
{
    HTTP_GET,
    HTTP_POST
}
HTTP_Method;


procedure begin_HTTP_Client_write(Output *output, HTTP_Method request_method, Byte *request);
    procedure add_HTTP_Client_field(Output *output, Byte *field, Byte *value);
procedure end_HTTP_Client_write(Output *output);


function N_32 begin_HTTP_Client_read(Input *input);
    function Boolean read_HTTP_Client_field(Input *input, Buffer *field, Buffer *value);
procedure end_HTTP_Client_read(Input *input);


procedure      write_HTTP_Client_GET (Output *output, Byte *path, Address *server_address);
function  N_32 read_HTTP_Client_GET  (Input *input, N_32 *content_length);

procedure      write_HTTP_Client_POST (Output *output, Byte *path, Buffer *data, Address *server_address);
function  N_32 read_HTTP_Client_POST  (Input *input, N_32 *content_length);


typedef struct
{
    TCP_Connection  connection;
    Address        *server_address;
}
HTTP_Client;


function Boolean initialize_HTTP_Client(HTTP_Client *client, Address *server_address);
    function N_32 initialize_HTTP_Client_GET_input(HTTP_Client *client, Byte *path, Input *input);
    function N_32 initialize_HTTP_Client_POST_input(HTTP_Client *client, Byte *path, Buffer *data, Input *input);
procedure deinitialize_HTTP_Client(HTTP_Client *client);


#include "HTTP.c"

#endif //HTTP_H_INCLUDED