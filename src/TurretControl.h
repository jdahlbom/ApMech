#ifndef AP_TURRET_CONTROL_H
#define AP_TURRET_CONTROL_H

#include "types.h"
#include "net/Controller.h"
#include <iostream>

namespace ap {

class TurretControl : public net::Controller {
    public:

    TurretControl();
    ~TurretControl() {}

    void reset();

    void setAimCoordinates(int x, int y);

    int serialize(uint8 buffer[], int start, int buflength) const;
    int unserialize(uint8 buffer[], int start);

    void dump();

    private:
    int x;
    int y;
}; // class TurretControl
} // namespace ap

#endif
