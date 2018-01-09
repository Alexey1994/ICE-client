#define left_rotate_32(a, n) (((a) << (n)) | ((a) >> (32 - (n))))
#define right_rotate_32(a, n) (((a) >> (n)) | ((a) << (32 - (n))))
/*
N_32 left_rotate_32(N_32 a, N_32 n)
{
    return (a << n) | (a >> 32 - n);
}


N_32 right_rotate_32(N_32 a, N_32 n)
{
    return (a >> n) | (a << 32 - n);
}*/


private procedure pad_SHA_data(Buffer *data, Boolean is_big_endian)
{
    N_32  i;
    N_64  data_length;
    N_64  bits_length;
    N_32  zeros_length;

    bits_length = buffer_length(data) * 8LL;

    if(is_big_endian)
        convert_big_to_little_endian(&bits_length, 8);

    write_in_buffer(data, 0b10000000);

    if(buffer_length(data)%64 > 56)
    {
        data_length = buffer_length(data);
        zeros_length = 64 - data_length%64;

        for(i=0; i<zeros_length; ++i)
            write_in_buffer(data, 0);

        for(i=0; i<56; ++i)
            write_in_buffer(data, 0);
    }
    else
    {
        data_length = buffer_length(data);
        zeros_length = 56 - data_length%64;

        for(i=0; i<zeros_length; ++i)
            write_in_buffer(data, 0);
    }

    for(i=0; i<8; ++i)
        write_in_buffer(data, ((Byte*)(&bits_length))[i]);
}


private procedure update_SHA_1_values(N_32 *values, N_32 block, N_32 f, N_32 k)
{
    N_32 temp;

    temp = left_rotate_32(values[0], 5) + f + values[4] + block + k;
    values[4] = values[3];
    values[3] = values[2];
    values[2] = left_rotate_32(values[1], 30);
    values[1] = values[0];
    values[0] = temp;
}


procedure calculate_SHA_1_hash(Buffer *data, N_32 *hash)
{
    N_64  i;
    N_64  j;
    N_32 *current_block;
    N_32  values[5];
    N_32  blocks[80];
    N_32  begin_index;
    N_32  end_index;

    hash[0] = 0x67452301;
    hash[1] = 0xEFCDAB89;
    hash[2] = 0x98BADCFE;
    hash[3] = 0x10325476;
    hash[4] = 0xC3D2E1F0;

    begin_index = data->begin_index;
    end_index = data->end_index;
    pad_SHA_data(data, 1);

    for(i=data->begin_index; i<data->end_index; i += 64)
    {
        current_block = &data->data[i];

        for(j=0; j<16; ++j)
        {
            blocks[j] = current_block[j];
            convert_big_to_little_endian(&blocks[j], 4);
        }

        for(; j<80; ++j)
            blocks[j] = left_rotate_32(blocks[j-3] ^ blocks[j-8] ^ blocks[j-14] ^ blocks[j-16], 1);

        for(j=0; j<5; ++j)
            values[j] = hash[j];

        for(j=0; j<20; ++j)
            update_SHA_1_values(
                values, blocks[j],
                (values[1] & values[2]) | ((~values[1]) & values[3]),
                0x5A827999
            );

        for(; j<40; ++j)
            update_SHA_1_values(
                values, blocks[j],
                values[1] ^ values[2] ^ values[3],
                0x6ED9EBA1
            );

        for(; j<60; ++j)
            update_SHA_1_values(
                values, blocks[j],
                (values[1] & values[2]) | (values[1] & values[3]) | (values[2] & values[3]),
                0x8F1BBCDC
            );

        for(; j<80; ++j)
            update_SHA_1_values(
                values, blocks[j],
                values[1] ^ values[2] ^ values[3],
                0xCA62C1D6
            );

        for(j=0; j<5; ++j)
            hash[j] += values[j];
    }

    data->begin_index = begin_index;
    data->end_index = end_index;

    for(i=0; i<5; ++i)
        convert_big_to_little_endian(&hash[i], 4);
}


N_32 SHA_256_constants[64] = {
    0x428A2F98, 0x71374491, 0xB5C0FBCF, 0xE9B5DBA5, 0x3956C25B, 0x59F111F1, 0x923F82A4, 0xAB1C5ED5,
    0xD807AA98, 0x12835B01, 0x243185BE, 0x550C7DC3, 0x72BE5D74, 0x80DEB1FE, 0x9BDC06A7, 0xC19BF174,
    0xE49B69C1, 0xEFBE4786, 0x0FC19DC6, 0x240CA1CC, 0x2DE92C6F, 0x4A7484AA, 0x5CB0A9DC, 0x76F988DA,
    0x983E5152, 0xA831C66D, 0xB00327C8, 0xBF597FC7, 0xC6E00BF3, 0xD5A79147, 0x06CA6351, 0x14292967,
    0x27B70A85, 0x2E1B2138, 0x4D2C6DFC, 0x53380D13, 0x650A7354, 0x766A0ABB, 0x81C2C92E, 0x92722C85,
    0xA2BFE8A1, 0xA81A664B, 0xC24B8B70, 0xC76C51A3, 0xD192E819, 0xD6990624, 0xF40E3585, 0x106AA070,
    0x19A4C116, 0x1E376C08, 0x2748774C, 0x34B0BCB5, 0x391C0CB3, 0x4ED8AA4A, 0x5B9CCA4F, 0x682E6FF3,
    0x748F82EE, 0x78A5636F, 0x84C87814, 0x8CC70208, 0x90BEFFFA, 0xA4506CEB, 0xBEF9A3F7, 0xC67178F2
};


procedure calculate_SHA_256_hash(Buffer *data, N_32 *hash)
{
    N_64  i;
    N_64  j;
    N_32 *current_block;
    N_32  values[8];
    N_32  blocks[64];
    N_32  begin_index;
    N_32  end_index;

    hash[0] = 0x6A09E667;
    hash[1] = 0xBB67AE85;
    hash[2] = 0x3C6EF372;
    hash[3] = 0xA54FF53A;
    hash[4] = 0x510E527F;
    hash[5] = 0x9B05688C;
    hash[6] = 0x1F83D9AB;
    hash[7] = 0x5BE0CD19;

    begin_index = data->begin_index;
    end_index = data->end_index;
    pad_SHA_data(data, 1);

    for(i=data->begin_index; i<data->end_index; i += 64)
    {
        current_block = &data->data[i];

        for(j=0; j<16; ++j)
        {
            blocks[j] = current_block[j];
            convert_big_to_little_endian(&blocks[j], 4);
        }

        for(; j<64; ++j)
        {
            N_32 s0 = right_rotate_32(blocks[j-15], 7) ^ right_rotate_32(blocks[j-15], 18) ^ (blocks[j-15] >> 3);
            N_32 s1 = right_rotate_32(blocks[j-2], 17) ^ right_rotate_32(blocks[j-2], 19) ^ (blocks[j-2] >> 10);
            blocks[j] = blocks[j-16] + s0 + blocks[j-7] + s1;
        }

        for(j=0; j<8; ++j)
            values[j] = hash[j];

        for(j=0; j<64; ++j)
        {
            N_32 Sigma0 = right_rotate_32(values[0], 2) ^ right_rotate_32(values[0], 13) ^ right_rotate_32(values[0], 22);
            N_32 Ma     = (values[0] & values[1]) ^ (values[0] & values[2]) ^ (values[1] & values[2]);
            N_32 temp2  = Sigma0 + Ma;
            N_32 Sigma1 = right_rotate_32(values[4], 6) ^ right_rotate_32(values[4], 11) ^ right_rotate_32(values[4], 25);
            N_32 Ch     = (values[4] & values[5]) ^ ( (~values[4]) & values[6] );
            N_32 temp1  = values[7] + Sigma1 + Ch + SHA_256_constants[j] + blocks[j];

            values[7] = values[6];
            values[6] = values[5];
            values[5] = values[4];
            values[4] = values[3] + temp1;
            values[3] = values[2];
            values[2] = values[1];
            values[1] = values[0];
            values[0] = temp1 + temp2;
        }

        for(j=0; j<8; ++j)
            hash[j] += values[j];
    }

    data->begin_index = begin_index;
    data->end_index = end_index;

    for(i=0; i<8; ++i)
        convert_big_to_little_endian(&hash[i], 4);
}


private N_32 MD5_s_constants[] = {
    7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
    5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
    4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
    6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21
};


private N_32 MD5_K_constants[] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};


private procedure update_MD5_values(N_32 *values, N_32 *block, N_8 i, N_32 F, N_32 g)
{
    F = F + values[0] + MD5_K_constants[i] + block[g];
    values[0] = values[3];
    values[3] = values[2];
    values[2] = values[1];
    values[1] = values[1] + left_rotate_32(F, MD5_s_constants[i]);
    /*N_32 temp;

    temp = values[3];
    values[3] = values[2];
    values[2] = values[1];
    values[1] = values[1] + left_rotate_32(values[0] + F + MD5_K_constants[i] + block[g], MD5_s_constants[i]);
    values[0] = temp;*/
}


procedure calculate_MD5_hash(Buffer *data, N_32 *hash)
{
    N_64  i;
    N_8   j;
    N_32 *current_block;
    N_32  begin_index;
    N_32  end_index;
    N_32  block[16];
    N_32  values[4];

    hash[0] = 0x67452301;
    hash[1] = 0xefcdab89;
    hash[2] = 0x98badcfe;
    hash[3] = 0x10325476;

    begin_index = data->begin_index;
    end_index = data->end_index;
    pad_SHA_data(data, 0);

    for(i=data->begin_index; i<data->end_index; i += 64)
    {
        current_block = &data->data[i];

        for(j=0; j<16; ++j)
            block[j] = current_block[j];

        for(j=0; j<4; ++j)
            values[j] = hash[j];

        for(j=0; j<16; ++j)
            update_MD5_values(
                values, block, j,
                (values[1] & values[2]) | ((~values[1]) & values[3]),
                j
            );

        for(; j<32; ++j)
            update_MD5_values(
                values, block, j,
                (values[3] & values[1]) | ((~values[3]) & values[2]),
                (j*5 + 1) % 16
            );

        for(; j<48; ++j)
            update_MD5_values(
                values, block, j,
                values[1] ^ values[2] ^ values[3],
                (j*3 + 5) % 16
            );

        for(; j<64; ++j)
            update_MD5_values(
                values, block, j,
                values[2] ^ (values[1] | (~values[3])),
                (j*7) % 16
            );

        for(j=0; j<4; ++j)
            hash[j] += values[j];
    }

    data->begin_index = begin_index;
    data->end_index = end_index;
}


procedure calculate_HMAC_SHA_1_hash(Buffer *data, Buffer *key, Byte *hash)
{
    Byte   SHA_1_hash[20];
    Byte   key_hash[20];
    Buffer new_key;
/*
    initialize_buffer(&new_key, 1);

    if(buffer_length(key) > 20)
    {
        calculate_SHA_1_hash(key, key_hash);
        //for(i=key->begin_index; i<key->end_index; ++i)
    }
    else
        for(i=buffer_length(key); i<20; ++i)
            key_hash[i] = ;*/
}
