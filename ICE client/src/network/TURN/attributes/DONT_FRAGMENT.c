private procedure add_DONT_FRAGMENT_attribute(TURN_Client *client)
{
    add_STUN_atribute_head(&client->STUN_client, DONT_FRAGMENT_TURN_ATTRIBUTE, 0);
}
