void generate_transaction_ID(Byte *transaction_ID)
{
    memset(transaction_ID, 0, 12);
    transaction_ID[0] = 0xb;
    transaction_ID[1] = 0xa;
    transaction_ID[2] = 0xb;
    transaction_ID[3] = 0xe;
}


void set_STUN_content_length(STUN_Head *head, int content_length)
{
    convert_big_to_little_endian(&content_length, 2);
    head->content_length = content_length;
}


String* create_STUN_head(unsigned short type)
{
    String    *head_data = create_string(200);
    STUN_Head *head      = head_data->begin;

    convert_big_to_little_endian(&type, 2);
    head->message_type   = type;
    head->magic_cookie   = STUN_COOKIE;
    generate_transaction_ID(head->transaction_ID);

    head_data->length = 20;

    return head_data;
}


void add_STUN_atribute_head(String *message, unsigned short type, int length)
{
    STUN_Attribute attribute;

    attribute.type   = type;
    attribute.length = length;

    convert_big_to_little_endian(&attribute.type, 2);
    convert_big_to_little_endian(&attribute.length, 2);

    write_byte_array_to_string(message, &attribute, 4);
}