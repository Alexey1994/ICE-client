#ifndef MD5_H_INCLUDED
#define MD5_H_INCLUDED


typedef unsigned char MD5_Hash[16];


void calculate_MD5_hash(MD5_Hash hash, String *string);
void print_MD5_hash(MD5_Hash hash);


#include "MD5.c"

#endif //MD5_H_INCLUDED