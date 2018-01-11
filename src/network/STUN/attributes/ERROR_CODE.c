void read_ERROR_CODE_attribute(STUN_Attributes *attributes, Byte *attribute, int length)
{
	attributes->ERROR_CODE = attribute[2] * 100 + attribute[3];
}
