#ifndef STUN_DEBUG_H_INCLUDED
#define STUN_DEBUG_H_INCLUDED


#include "debug.h"


#define ENABLE_STUN_DEBUG 1


void initialize_STUN_debug();

void print_MAPPED_ADDRESS_attribute(Byte *attribute, int length);
void print_RESPONSE_ADDRESS_attribute(Byte *attribute, int length);
void print_CHANGE_REQUEST_attribute(Byte *attribute, int length);
void print_SOURCE_ADDRESS_attribute(Byte *attribute, int length);
void print_CHANGED_ADDRESS_attribute(Byte *attribute, int length);
void print_USERNAME_attribute(Byte *attribute, int length);
void print_PASSWORD_attribute(Byte *attribute, int length);
void print_MESSAGE_INTEGRITY_attribute(Byte *attribute, int length);
void print_ERROR_CODE_attribute(Byte *attribute, int length);
void print_UNKNOWN_ATTRIBUTES_attribute(Byte *attribute, int length);
void print_REFLECTED_FROM_attribute(Byte *attribute, int length);
void print_REALM_attribute(Byte *attribute, int length);
void print_NONCE_attribute(Byte *attribute, int length);
void print_XOR_MAPPED_ADDRESS_attribute(Byte *attribute, int length);
void print_SOFTWARE_attribute(Byte *attribute, int length);
void print_ALTERNATE_SERVER_attribute(Byte *attribute, int length);
void print_FINGERPRINT_attribute(Byte *attribute, int length);

void print_STUN_attribute (STUN_Attribute *attribute);
void print_STUN_attributes (String *message);
void print_STUN_head (STUN_Header *header);
void print_STUN_request (String *message);
void print_STUN_response (String *message);


#endif //STUN_DEBUG_H_INCLUDED
