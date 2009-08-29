#ifndef AP_NABLACONTROL_H
#define AP_NABLACONTROL_H

#include "net/Controller.h"
#include "types.h"

namespace ap
{


class NablaControl : public ap::net::Controller
{
 public:

    float a, turning;       // m/s, and rad/s
    unsigned int controls;
    int color;

    NablaControl();
    ~NablaControl() {};

    int serialize(uint8 buffer[], int start, int buflength) const;
    int unserialize(uint8 buffer[], int start);
};

} // namespace ap

#endif
