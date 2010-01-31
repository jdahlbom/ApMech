#include "TurretControl.h"

#include "net/serializer.h"

namespace ap {
TurretControl::TurretControl():
    x(0), y(0) {}

void TurretControl::reset()
{
    x = y = 0;
}

int TurretControl::serialize(uint8 buffer[], int start, int buflength) const {
    return ap::net::serialize(x, buffer, start, buflength);
    return ap::net::serialize(y, buffer, start, buflength);
}

int TurretControl::unserialize(uint8 buffer[], int start) {
    return ap::net::unserialize(x, buffer, start);
    return ap::net::unserialize(y, buffer, start);
}

void TurretControl::setAimCoordinates(int aimX, int aimY) {
    x = aimX;
    y = aimY;
}

void TurretControl::dump() {
    std::cout << "[TURRETCONTROL::dump] aim: (" << ((int)x) << "," << ((int)y) << ")" << std::endl;
}

} // namespace ap
