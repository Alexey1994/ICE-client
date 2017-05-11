#include "debug.h"
#include "../../logger/logger.h"


static void (*print_attribute_handlers[65536])(Byte *attribute, int attribute_length);


void initialize_STUN_debug()
{
    int i;

    for(i=0; i<65536; ++i)
        print_attribute_handlers[i] = 0;

    print_attribute_handlers[MAPPED_ADDRESS]     = print_MAPPED_ADDRESS_attribute;
    print_attribute_handlers[RESPONSE_ADDRESS]   = print_RESPONSE_ADDRESS_attribute;
    print_attribute_handlers[CHANGE_REQUEST]     = print_CHANGE_REQUEST_attribute;
    print_attribute_handlers[SOURCE_ADDRESS]     = print_SOURCE_ADDRESS_attribute;
    print_attribute_handlers[CHANGED_ADDRESS]    = print_CHANGED_ADDRESS_attribute;
    print_attribute_handlers[USERNAME]           = print_USERNAME_attribute;
    print_attribute_handlers[PASSWORD]           = print_PASSWORD_attribute;
    print_attribute_handlers[MESSAGE_INTEGRITY]  = print_MESSAGE_INTEGRITY_attribute;
    print_attribute_handlers[ERROR_CODE]         = print_ERROR_CODE_attribute;
    print_attribute_handlers[UNKNOWN_ATTRIBUTES] = print_UNKNOWN_ATTRIBUTES_attribute;
    print_attribute_handlers[REFLECTED_FROM]     = print_REFLECTED_FROM_attribute;
    print_attribute_handlers[REALM]              = print_REALM_attribute;
    print_attribute_handlers[NONCE]              = print_NONCE_attribute;
    print_attribute_handlers[XOR_MAPPED_ADDRESS] = print_XOR_MAPPED_ADDRESS_attribute;
    print_attribute_handlers[SOFTWARE]           = print_SOFTWARE_attribute;
    print_attribute_handlers[ALTERNATE_SERVER]   = print_ALTERNATE_SERVER_attribute;
    print_attribute_handlers[FINGERPRINT]        = print_FINGERPRINT_attribute;
}


void print_MAPPED_ADDRESS_attribute(Byte *attribute, int length)
{
    unsigned int   ip;
    unsigned short port;

    char logbuf[200];
    snprintf(logbuf, 200, "\tMAPPED ADDRESS, %d bytes\n", length);
    print_log(logbuf);
    print_log("\t");

    switch(attribute[1])
    {
        case 1: print_log("IPv4\n"); break;
        case 2: print_log("IPv6\n"); break;
    }

    port = (attribute[2]<<8) + attribute[3];
    ip   = (attribute[4]<<24) + (attribute[5]<<16) + (attribute[6]<<8) + (attribute[7]);

    snprintf(logbuf, 200, "\t%d.%d.%d.%d:%d\n", attribute[4], attribute[5], attribute[6], attribute[7], port);
    print_log(logbuf);

    print_log("\n");
}


void print_RESPONSE_ADDRESS_attribute(Byte *attribute, int length)
{
    char logbuf[200];
    snprintf(logbuf, 200, "\tRESPONSE ADDRESS, %d bytes\n", length);
    print_log(logbuf);
}


void print_CHANGE_REQUEST_attribute(Byte *attribute, int length)
{
    char logbuf[200];
    snprintf(logbuf, 200, "\tCHANGE REQUEST, %d bytes\n", length);
    print_log(logbuf);
}


void print_SOURCE_ADDRESS_attribute(Byte *attribute, int length)
{
    unsigned int   ip;
    unsigned short port;

    char logbuf[200];
    snprintf(logbuf, 200, "\tSOURCE ADDRESS, %d bytes\n", length);
    print_log(logbuf);

    print_log("\t");

    switch(attribute[1])
    {
        case 1: print_log("IPv4\n"); break;
        case 2: print_log("IPv6\n"); break;
    }

    port = (attribute[2]<<8) + attribute[3];
    ip   = (attribute[4]<<24) + (attribute[5]<<16) + (attribute[6]<<8) + (attribute[7]);

    snprintf(logbuf, 200, "\t%d.%d.%d.%d:%d\n", attribute[4], attribute[5], attribute[6], attribute[7], port);
    print_log(logbuf);

    print_log("\n");
}


void print_CHANGED_ADDRESS_attribute(Byte *attribute, int length)
{
    unsigned int   ip;
    unsigned short port;

    char logbuf[200];
    snprintf(logbuf, 200, "\tCHANGED ADDRESS, %d bytes\n", length);
    print_log(logbuf);

    print_log("\t");

    switch(attribute[1])
    {
        case 1: print_log("IPv4\n"); break;
        case 2: print_log("IPv6\n"); break;
    }

    port = (attribute[2]<<8) + attribute[3];
    ip   = (attribute[4]<<24) + (attribute[5]<<16) + (attribute[6]<<8) + (attribute[7]);

    snprintf(logbuf, 200, "\t%d.%d.%d.%d:%d\n", attribute[4], attribute[5], attribute[6], attribute[7], port);
    print_log(logbuf);

    print_log("\n");
}


void print_USERNAME_attribute(Byte *attribute, int length)
{
    char logbuf[200];
    snprintf(logbuf, 200, "\tUSERNAME, %d bytes\n", length);
    print_log(logbuf);
}


void print_PASSWORD_attribute(Byte *attribute, int length)
{
    char logbuf[200];
    snprintf(logbuf, 200, "\tPASSWORD, %d bytes\n", length);
    print_log(logbuf);
}


void print_MESSAGE_INTEGRITY_attribute(Byte *attribute, int length)
{
    char logbuf[200];
    snprintf(logbuf, 200, "\tMESSAGE INTEGRITY, %d bytes\n", length);
    print_log(logbuf);
}


void print_ERROR_CODE_attribute(Byte *attribute, int length)
{
    char logbuf[200];
    snprintf(logbuf, 200, "\tERROR CODE, %d bytes\n", length);
    print_log(logbuf);
}


void print_UNKNOWN_ATTRIBUTES_attribute(Byte *attribute, int length)
{
    char logbuf[200];
    snprintf(logbuf, 200, "\tUNKNOWN ATTRIBUTES, %d bytes\n", length);
    print_log(logbuf);
}


void print_REFLECTED_FROM_attribute(Byte *attribute, int length)
{
    char logbuf[200];
    snprintf(logbuf, 200, "\tREFLECTED FROM, %d bytes\n", length);
    print_log(logbuf);
}


void print_REALM_attribute(Byte *attribute, int length)
{
    char logbuf[200];
    snprintf(logbuf, 200, "\tREALM, %d bytes\n", length);
    print_log(logbuf);
}


void print_NONCE_attribute(Byte *attribute, int length)
{
    char logbuf[200];
    snprintf(logbuf, 200, "\tNONCE, %d bytes\n", length);
    print_log(logbuf);
}


void print_XOR_MAPPED_ADDRESS_attribute(Byte *attribute, int length)
{
    char logbuf[200];
    snprintf(logbuf, 200, "\tXOR MAPPED ADDRESS, %d bytes\n", length);
    print_log(logbuf);
}


void print_SOFTWARE_attribute(Byte *attribute, int length)
{
    int i;

    char logbuf[200];
    snprintf(logbuf, 200, "\tSOFTWARE, %d bytes\n", length);
    print_log(logbuf);

    for(i=0; i<length; ++i)
    {
        snprintf(logbuf, 200, "%c", attribute[i]);
        print_log(logbuf);
    }

    print_log("\n");
}


void print_ALTERNATE_SERVER_attribute(Byte *attribute, int length)
{
    char logbuf[200];
    snprintf(logbuf, 200, "\tALTERNATE SERVER, %d bytes\n", length);
    print_log(logbuf);
}


void print_FINGERPRINT_attribute(Byte *attribute, int length)
{
    char logbuf[200];
    snprintf(logbuf, 200, "\tFINGERPRINT, %d bytes\n", length);
    print_log(logbuf);
}


void print_STUN_attribute(STUN_Attribute *attribute)
{
    void (*attribute_handler)(Byte *attribute, int attribute_length);

    attribute_handler = print_attribute_handlers[ attribute->type ];

    if(!attribute_handler)
    {
        char logbuf[200];
        snprintf(logbuf, 200, "\tunknow attribute 0x%02x, %d bytes\n\n", attribute->type, attribute->length);
        print_log(logbuf);
        return;
    }

    attribute_handler((Byte*)attribute + 4, attribute->length);
    print_log("\n");
}


void print_STUN_attributes(String *message)
{
    int             length     = 20;
    Byte           *attributes = message->begin + 20;
    STUN_Attribute *attribute  = attributes;

    print_log("\n\tAttributes:\n\n");

    while(length < message->length)
    {
        print_STUN_attribute(attribute);
        length += 4 + attribute->length;
        attribute = (Byte*)attribute + 4 + attribute->length;
    }
}


void print_STUN_head(STUN_Header *header)
{
    print_log("Type:            ");

    switch(header->message_type)
    {
        case BINDING_REQUEST:        print_log("REQUEST\n");                break;
        case BINDING_RESPONSE:       print_log("SERVER RESPONSE\n");        break;
        case BINDING_ERROR_RESPONSE: print_log("SERVER RESPONSE ERROR\n");  break;
    }

    char logbuf[200];
    snprintf(logbuf, 200, "Length:          %d\n", header->message_length);
    print_log(logbuf);

    snprintf(logbuf, 200, "magic:           0x%04x\n", header->magic_cookie);
    print_log(logbuf);

    snprintf(logbuf, 200, "transaction ID:  0x%01x%01x%01x%01x%01x%01x%01x%01x%01x%01x%01x%01x\n",
        header->transaction_ID[0],
        header->transaction_ID[1],
        header->transaction_ID[2],
        header->transaction_ID[3],
        header->transaction_ID[4],
        header->transaction_ID[5],
        header->transaction_ID[6],
        header->transaction_ID[7],
        header->transaction_ID[8],
        header->transaction_ID[9],
        header->transaction_ID[10],
        header->transaction_ID[11]);

    print_log(logbuf);
}


void print_STUN_request(String *message)
{
    print_log("Request\n\n");
    print_STUN_head(message->begin);
    print_log("\n");
}


void print_STUN_response(String *message)
{
    print_log("Response\n\n");
    print_STUN_head(message->begin);
    print_STUN_attributes(message);
    print_log("\n");
}
