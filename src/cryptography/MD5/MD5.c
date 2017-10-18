void calculate_MD5_hash(MD5_Hash hash, String *string)
{
	
}


void print_MD5_hash(MD5_Hash hash)
{
	int i;

	for(i=0; i<16; ++i)
		printf("%x", hash[i]);
}