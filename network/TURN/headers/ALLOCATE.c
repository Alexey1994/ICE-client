void add_ALLOCATE_request_head(String *message, int content_length)
{
    STUN_Header *header      =  message->begin;

    header->message_type   = ALLOCATE_TURN_MESSAGE;
    header->message_length = content_length;
    header->magic_cookie   = STUN_COOKIE;
    generate_transaction_ID(header->transaction_ID);

    message->length = 20;
}