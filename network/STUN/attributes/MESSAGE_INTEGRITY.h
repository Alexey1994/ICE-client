#ifndef MESSAGE_INTEGRITY_STUN_ATTRIBUTE_H_INCLUDED
#define MESSAGE_INTEGRITY_STUN_ATTRIBUTE_H_INCLUDED


#include "../../../extends/extends.h"
#include "../STUN.h"


void read_MESSAGE_INTEGRITY_attribute(STUN_Attributes *attributes, Byte *attribute, int length);


#endif // MESSAGE_INTEGRITY_STUN_ATTRIBUTE_H_INCLUDED
