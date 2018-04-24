private function Byte convert_byte_to_base64(Byte byte)
{
    if(byte < 26)
        return byte + 'A';

    if(byte < 52)
        return byte + 'a' - 26;

    if(byte < 62)
        return byte + '0' - 52;

    if(byte == 62)
        return '+';

    return '/';
}


internal procedure begin_base64_encoder(Base64_Coder *encoder, Byte *output, procedure (*write_byte)(Byte *output, Byte byte))
{
    encoder->position       = 2;
    encoder->previouse_byte = 0;
    encoder->output         = output;
    encoder->write_byte     = write_byte;
}


internal procedure encode_base64(Base64_Coder *encoder, Byte byte)
{
    encoder->write_byte(encoder->output, convert_byte_to_base64( (byte >> encoder->position) | encoder->previouse_byte) );
    encoder->previouse_byte = (byte & (0b00111111 >> (6 - encoder->position))) << (6 - encoder->position);

    if(encoder->position == 6)
    {
        encoder->write_byte(encoder->output, convert_byte_to_base64(encoder->previouse_byte));
        encoder->position = 2;
        encoder->previouse_byte = 0;
    }
    else
        encoder->position = encoder->position + 2;
}


internal procedure end_base64_encoder(Base64_Coder *encoder)
{
    if(encoder->position != 2)
    {
        encoder->write_byte(encoder->output, convert_byte_to_base64(encoder->previouse_byte));

        if(encoder->position == 4)
        {
            encoder->write_byte(encoder->output, '=');
            encoder->write_byte(encoder->output, '=');
        }
        else
            encoder->write_byte(encoder->output, '=');
    }
}


private function Byte convert_byte_from_base64(Byte byte)
{
    if(byte == '=')
        return 0;

    if(byte == '/')
        return 63;

    if(byte == '+')
        return 62;

    if(byte >= 'A' && byte <= 'Z')
        return byte - 'A';

    if(byte >= 'a' && byte <= 'z')
        return byte - 'a' + 26;

    return byte - '0' + 52;
}


internal procedure begin_base64_decoder(Base64_Coder *decoder, Byte *output, procedure (*write_byte)(Byte *output, Byte byte))
{
    decoder->position       = 0;
    decoder->previouse_byte = 0;
    decoder->output         = output;
    decoder->write_byte     = write_byte;
}


internal procedure decode_base64(Base64_Coder *decoder, Byte byte)
{
    byte = convert_byte_from_base64(byte);

    if(decoder->position)
        decoder->write_byte(decoder->output, (decoder->previouse_byte << decoder->position) | (byte >> (6 - decoder->position)));

    decoder->previouse_byte = byte & (0b00111111 >> decoder->position);
    decoder->position += 2;

    if(decoder->position == 8)
    {
        decoder->position       = 0;
        decoder->previouse_byte = 0;
    }
}


internal function Boolean end_base64_decoder(Base64_Coder *decoder)
{
    if(decoder->position == 2)
        goto error;

    return 1;

error:
    return 0;
}