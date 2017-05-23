int main(int arguments_length, char *arguments[])
{
    char           mapped_host[16];
    unsigned short mapped_port;

    initialize_STUN();
    initialize_TURN();
/*
    if(!get_STUN_mapped_address("127.0.0.1", 3478, mapped_host, &mapped_port))
        return 0;
*/
    if(!get_STUN_mapped_address("127.0.0.1", 3478, mapped_host, &mapped_port))
        return 0;

    return 0;
}