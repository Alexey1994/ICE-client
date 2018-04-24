private N_32 seed   = 0x67452301;
private N_32 seed_L = 0xefcdab89;


internal function N_32 random_number()
{
    seed = seed * seed_L + current_time();

    return seed;
}