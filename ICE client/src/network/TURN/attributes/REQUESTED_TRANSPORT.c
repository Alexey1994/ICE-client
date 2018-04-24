//      0                   1                   2                   3
//      0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//     |    Protocol   |                    RFFU                       |
//     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+


private procedure add_REQUESTED_TRANSPORT_attribute (TURN_Client *client, Byte transport_type)
{
    add_STUN_atribute_head(&client->STUN_client, REQUESTED_TRANSPORT_TURN_ATTRIBUTE, 4);

    write_byte(&client->STUN_client.buffer_output, 17); //only UDP

    /*
    switch(transport_type)
    {
        case UDP_CONNECTION: write_byte(&client->STUN_client.buffer_output, 17); break;
        case TCP_CONNECTION: write_byte(&client->STUN_client.buffer_output, 17); break;

        default:
            write_byte(&client->STUN_client.buffer_output, 17);
    }*/

    write_byte(&client->STUN_client.buffer_output, 0);
    write_byte(&client->STUN_client.buffer_output, 0);
    write_byte(&client->STUN_client.buffer_output, 0);
}
