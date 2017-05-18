#ifndef TURN_DEBUG_H_INCLUDED
#define TURN_DEBUG_H_INCLUDED


#include "debug.h"


#define ENABLE_TURN_DEBUG 0


void initialize_TURN_debug();

void print_DATA_attribute(Byte *attribute, int length);


void print_TURN_head(STUN_Head *head);
void print_TURN_response(String *message);
void print_TURN_request(String *message);


#if ENABLE_TURN_DEBUG
# include "debug.c"
#endif


#endif //TURN_DEBUG_H_INCLUDED
