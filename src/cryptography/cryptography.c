#define left_rotate_32(a, n) (((a) << (n)) | ((a) >> (sizeof(a)*8 - n)))
#define right_rotate_32(a, n) (((a) >> (n)) | ((a) << (sizeof(a)*8 - n)))
/*
N_32 left_rotate_32(N_32 a, N_32 n)
{
    return (a << n) | (a >> 32 - n);
}


N_32 right_rotate_32(N_32 a, N_32 n)
{
    return (a >> n) | (a << 32 - n);
}*/


private procedure pad_SHA_data(Buffer *data)
{
    N_32  i;
    N_64  data_length;
    N_64  bits_length;
    N_32  zeros_length;

    bits_length = buffer_length(data) * 8LL;
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
    pad_SHA_data(data);

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
    pad_SHA_data(data);

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


procedure calculate_HMAC_SHA_1_hash(Buffer *data, Buffer *key, Byte *hash)
{
    Byte SHA_1_hash[20];
}