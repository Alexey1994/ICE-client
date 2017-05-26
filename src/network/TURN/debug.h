#ifndef TURN_DEBUG_H_INCLUDED
#define TURN_DEBUG_H_INCLUDED


#include "debug.h"


void initialize_TURN_debug();


void print_TURN_head(STUN_Head *head);
void print_TURN_response(String *message);
void print_TURN_request(String *message);


#ifdef ENABLE_DEBUG
# include "debug.c"
#endif


#endif //TURN_DEBUG_H_INCLUDED
