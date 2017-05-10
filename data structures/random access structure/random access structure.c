#include "random access structure.h"


RandomAccessStructure* create_random_access_structure(Byte *structure,
                                                      Byte* (*get_element)(RandomAccessStructure *structure, int pos),
                                                      int (*get_length)(RandomAccessStructure *structure))
{
    RandomAccessStructure *random_access_structure = new(RandomAccessStructure);

    random_access_structure->structure   = structure;
    random_access_structure->get_element = get_element;
    random_access_structure->get_length  = get_length;

    return random_access_structure;
}


void destroy_random_random_access_structure(RandomAccessStructure *structure)
{
    free(structure);
}
