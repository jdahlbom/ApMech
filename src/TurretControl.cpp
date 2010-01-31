#include "TurretControl.h"

#ifndef WIN32
#include <Ogre.h>
#else
#include <Ogre/Ogre.h>
#endif

#include "net/serializer.h"

namespace ap {
TurretControl::TurretControl():
    aim(Ogre::Vector3(0,0,0)) {}

void TurretControl::reset()
{
    aim = Ogre::Vector3(0,0,0);
}

int TurretControl::serialize(uint8 buffer[], int start, int buflength) const {
    return ap::net::serialize(aim, buffer, start, buflength);
}

int TurretControl::unserialize(uint8 buffer[], int start) {
    return ap::net::unserialize(aim, buffer, start);
}

void TurretControl::setAimCoordinates(Ogre::Vector3 target) {
    aim = target;
}

void TurretControl::dump() {
    std::cout << "[TURRETCONTROL::dump] aim: (" << ((float)aim.x) << "," << ((float)aim.z) << ")" << std::endl;
}

} // namespace ap
