#include "debug.h"
#include "../../logger/logger.h"


static void (*print_TURN_attribute_handlers[65536])(Byte *attribute, int attribute_length);


void print_DATA_attribute(Byte *attribute, int length)
{
    char logbuf[200];
    snprintf(logbuf, 200, "\tRESPONSE ADDRESS, %d bytes\n", length);
    print_log(logbuf);
}


void print_REQUESTED_TRANSPORT_attribute(Byte *attribute, int length)
{
    convert_big_to_little_endian(&length, 2);

    char logbuf[200];
    snprintf(logbuf, 200, "\tREQUESTED TRANSPORT, %d bytes\n", length);
    print_log(logbuf);

    switch(*attribute)
    {
        case 17: print_log("\tUDP connection type\n"); break;

        default:
            print_log("\tunknown connection type\n");
    }
}


void initialize_TURN_debug()
{
    int i;

    for(i=0; i<65536; ++i)
        print_TURN_attribute_handlers[i] = 0;

    print_TURN_attribute_handlers[DATA_TURN_ATTRIBUTE]                = print_DATA_attribute;
    print_TURN_attribute_handlers[0x1900] = print_REQUESTED_TRANSPORT_attribute;
}


void print_TURN_attribute(TURN_Attribute *attribute)
{
    void (*attribute_handler)(Byte *attribute, int attribute_length);

    attribute_handler = print_TURN_attribute_handlers[ attribute->type ];

    if(!attribute_handler)
    {
        attribute_handler = print_STUN_attribute_handlers[ attribute->type ];

        if(!attribute_handler)
        {
            char logbuf[200];
            snprintf(logbuf, 200, "\tunknow attribute 0x%02x, %d bytes\n\n", attribute->type, attribute->length);
            print_log(logbuf);
            return;
        }
    }

    attribute_handler((Byte*)attribute + 4, attribute->length);
    print_log("\n");
}


void print_TURN_attributes(String *message)
{
    int             length     = 20;
    Byte           *attributes = message->begin + 20;
    TURN_Attribute *attribute  = attributes;

    print_log("\n\tAttributes:\n\n");

    while(length < message->length)
    {
        print_TURN_attribute(attribute);
        length += 4 + attribute->length;
        attribute = (Byte*)attribute + 4 + attribute->length;
    }

    //print_STUN_attributes(message);
}


void print_TURN_head(STUN_Head *head)
{
    char logbuf[200];
    print_log("\tType:            ");

    switch(head->message_type)
    {
        case BINDING_REQUEST:                print_log("REQUEST\n");                break;
        case BINDING_RESPONSE:               print_log("SERVER RESPONSE\n");        break;
        case BINDING_ERROR_RESPONSE:         print_log("SERVER RESPONSE ERROR\n");  break;

        case ALLOCATE_TURN_MESSAGE:          print_log("ALLOCATE\n");               break;
        case REFRESH_TURN_MESSAGE:           print_log("REFRESH\n");                break;
        case SEND_TURN_MESSAGE:              print_log("SEND\n");                   break;
        case DATA_TURN_MESSAGE:              print_log("DATA\n");                   break;
        case CREATE_PERMISSION_TURN_MESSAGE: print_log("CREATE PERMISSION\n");      break;
        case CHANNEL_BIND_TURN_MESSAGE:      print_log("CHANNEL BIND\n");           break;

        default:
            snprintf(logbuf, 200, "unknown type %d\n", head->message_type);
            print_log(logbuf);
    }

    snprintf(logbuf, 200, "\tLength:          %d\n", head->content_length);
    print_log(logbuf);

    snprintf(logbuf, 200, "\tmagic:           0x%04x\n", head->magic_cookie);
    print_log(logbuf);

    snprintf(logbuf, 200, "\ttransaction ID:  0x%01x%01x%01x%01x%01x%01x%01x%01x%01x%01x%01x%01x\n",
        head->transaction_ID[0],
        head->transaction_ID[1],
        head->transaction_ID[2],
        head->transaction_ID[3],
        head->transaction_ID[4],
        head->transaction_ID[5],
        head->transaction_ID[6],
        head->transaction_ID[7],
        head->transaction_ID[8],
        head->transaction_ID[9],
        head->transaction_ID[10],
        head->transaction_ID[11]);

    print_log(logbuf);
}


void print_TURN_request(String *message)
{
    print_log("TURN Request\n\n");
    print_TURN_head(message->begin);
    print_TURN_attributes(message);
    print_log("\n");
}


void print_TURN_response(String *message)
{
    print_log("TURN Response\n\n");
    print_TURN_head(message->begin);
    print_TURN_attributes(message);
    print_log("\n");
}
