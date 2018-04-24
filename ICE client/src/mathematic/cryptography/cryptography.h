#ifndef CRYPTOGRAPHY_H_INCLUDED
#define CRYPTOGRAPHY_H_INCLUDED


#include "../../data structures/buffer/buffer.h"


internal procedure calculate_SHA_1_hash      (Buffer *data, N_32 *hash);
internal procedure calculate_MD5_hash        (Buffer *data, N_32 *hash);
internal procedure calculate_HMAC_SHA_1_hash (Buffer *data, Buffer *key, Byte *hash);


#include "cryptography.c"

#endif // CRYPTOGRAPHY_H_INCLUDED
