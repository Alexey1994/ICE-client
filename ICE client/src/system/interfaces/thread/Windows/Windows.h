typedef struct
{
    N_32     length;
    Byte    *security_descriptor;
    Boolean  is_inherit_handle;
}
Windows_Sequrity_Attributes;


private stdcall procedure Sleep        (N_32 milliseconds);
private stdcall function  Byte* CreateThread (
    Windows_Sequrity_Attributes *security_attributes,
    N_32                         stack_size,
    procedure                  (*thread_function)(Byte *arguments),
    Byte                        *attributes,
    N_32                         creation_flags,
    N_32                        *thread_id
);

#include "Windows.c"
