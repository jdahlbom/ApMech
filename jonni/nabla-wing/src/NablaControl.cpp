#include "NablaControl.h"

namespace ap {

NablaControl::NablaControl()
{
    color = 0x00FFFFFF; controls=0;
    a = turning = 0.0f;
}

int NablaControl::serialize(uint8 buffer[], int start, int buflength) const
{
    int length = 0;
    *(int *)(buffer+start+length) = color;                  length += 4;
    *(float *)(buffer+start+length) = a;                    length += 4;
    *(float *)(buffer+start+length) = turning;              length += 4;
    *(unsigned int *)(buffer+start+length) = controls;      length += 4;
    return length;
}

int NablaControl::unserialize(uint8 buffer[], int start)
{
    int length = 0;
    color = *(int *)(buffer+start+length);              length += 4;
    a = *(float *)(buffer+start+length);                length += 4;
    turning = *(float *)(buffer+start+length);          length += 4;
    controls = *(unsigned int *)(buffer+start+length);  length += 4;
    return length;
}

} // namespace ap
