#ifndef CRYPTOGRAPHY_H_INCLUDED
#define CRYPTOGRAPHY_H_INCLUDED


#include "../data structures/buffer/buffer.h"


procedure calculate_SHA_1_hash(Buffer *data, N_32 *hash);


#include "cryptography.c"

#endif // CRYPTOGRAPHY_H_INCLUDED
