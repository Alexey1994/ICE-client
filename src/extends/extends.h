#ifndef EXTENDS_H_INCLUDED
#define EXTENDS_H_INCLUDED


#define private static


#define loop for(;;)
#define inc ++
#define procedure void
#define function


typedef char                   Z_8;
typedef short int              Z_16;
typedef long int               Z_32;
typedef long long int          Z_64;

typedef unsigned char          N_8;
typedef unsigned short int     N_16;
typedef unsigned long int      N_32;
typedef unsigned long long int N_64;

typedef float                  R_32;
typedef long                   R_64;

typedef unsigned char Byte;
typedef unsigned char Boolean;
typedef unsigned int  Pointer;


typedef struct
{
    Byte *data;
    Byte  type;
}
DynamicData;


#define new(type) malloc(sizeof(type))
#define new_array(type, length) malloc(sizeof(type)*length)

#define reallocate(data, type) data=realloc(data, sizeof(type))
#define reallocate_array(data, type, length) data=realloc(data, sizeof(type)*length)


DynamicData* create_dynamic_data(Byte *data, Byte type);
void         destroy_dynamic_data(DynamicData *data);

void         convert_big_to_little_endian (Byte *array, int length);
procedure    swap                         (Byte *element1, Byte *element2, N_32 length);


#include "extends.c"

#endif // EXTENDS_H_INCLUDED
