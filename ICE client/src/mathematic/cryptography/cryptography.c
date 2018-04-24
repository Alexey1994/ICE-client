#define left_rotate_32(a, n) (((a) << (n)) | ((a) >> (32 - (n))))
#define right_rotate_32(a, n) (((a) >> (n)) | ((a) << (32 - (n))))


private procedure pad_SHA_data(Buffer *data, Boolean is_big_endian)
{
    N_32  i;
    N_64  data_length;
    N_64  bits_length;
    N_32  zeros_length;

    bits_length = buffer_length(data) * 8LL;

    if(is_big_endian)
        convert_little_to_big_endian(&bits_length, 8);
    else
        convert_big_to_little_endian(&bits_length, 8);

    write_in_buffer(data, 0b10000000);

    if((buffer_length(data) & 63) > 56)
    {
        data_length = buffer_length(data);
        zeros_length = 64 - (data_length & 63);

        for(i=0; i<zeros_length; ++i)
            write_in_buffer(data, 0);

        for(i=0; i<56; ++i)
            write_in_buffer(data, 0);
    }
    else
    {
        data_length = buffer_length(data);
        zeros_length = 56 - (data_length & 63);

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


internal procedure calculate_SHA_1_hash(Buffer *data, N_32 *hash)
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
            convert_little_to_big_endian(&blocks[j], 4);
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
        convert_little_to_big_endian(&hash[i], 4);
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
}


internal procedure calculate_MD5_hash(Buffer *data, N_32 *hash)
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
                (j*5 + 1) & 15
            );

        for(; j<48; ++j)
            update_MD5_values(
                values, block, j,
                values[1] ^ values[2] ^ values[3],
                (j*3 + 5) & 15
            );

        for(; j<64; ++j)
            update_MD5_values(
                values, block, j,
                values[2] ^ (values[1] | (~values[3])),
                (j*7) & 15
            );

        for(j=0; j<4; ++j)
            hash[j] += values[j];
    }

    data->begin_index = begin_index;
    data->end_index = end_index;

    for(i=0; i<4; ++i)
        convert_big_to_little_endian(&hash[i], 4);
}


internal procedure calculate_HMAC_SHA_1_hash(Buffer *data, Buffer *key, Byte *hash)
{
    N_32   i;
    Byte   SHA_1_hash[20];
    Byte   new_key[64];
    Byte   inner_hash[20];
    Buffer inner_data;
    Output inner_data_output;
    Buffer outer_data;
    Output outer_data_output;

    initialize_buffer(&inner_data, 1);
    initialize_buffer_output(&inner_data, &inner_data_output);

    initialize_buffer(&outer_data, 1);
    initialize_buffer_output(&outer_data, &outer_data_output);

    if(buffer_length(key) <= 64)
    {
        for(i=key->begin_index; i<key->end_index; ++i)
            new_key[i] = key->data[i];

        for(i=key->end_index; i<64; ++i)
            new_key[i] = 0;
    }
    else
    {
        calculate_SHA_1_hash(key, new_key);

        for(i=20; i<64; ++i)
            new_key[i] = 0;
    }

    for(i=0; i<64; ++i)
        write_byte(&inner_data_output, new_key[i] ^ 0x36);

    for(i=data->begin_index; i<data->end_index; ++i)
        write_byte(&inner_data_output, data->data[i]);

    calculate_SHA_1_hash(&inner_data, inner_hash);

    for(i=0; i<64; ++i)
        write_byte(&outer_data_output, new_key[i] ^ 0x5c);

    for(i=0; i<20; ++i)
        write_byte(&outer_data_output, inner_hash[i]);

    calculate_SHA_1_hash(&outer_data, hash);

    deinitialize_buffer(&inner_data);
    deinitialize_buffer(&outer_data);
}
