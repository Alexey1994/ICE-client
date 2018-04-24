#ifndef SDP_H_INCLUDED
#define SDP_H_INCLUDED


typedef struct
{

}
SDP_Data;


internal procedure parse_SDP_message (SDP_Data *data, Input *input);
internal procedure begin_SDP_message (Output *output);
internal procedure add_SDP_candidate (Output *output);
internal procedure end_SDP_message   (Output *output);


#endif //SDP_H_INCLUDED