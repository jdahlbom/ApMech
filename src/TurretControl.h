#ifndef AP_TURRET_CONTROL_H
#define AP_TURRET_CONTROL_H

#ifndef WIN32
#include <Ogre.h>
#else
#include <Ogre/Ogre.h>
#endif

#include "types.h"
#include "net/Controller.h"
#include <iostream>

namespace ap {

class TurretControl : public net::Controller {
    public:

    TurretControl();
    ~TurretControl() {}

    void reset();

    void setAimCoordinates(Ogre::Vector3 target);

    int serialize(uint8 buffer[], int start, int buflength) const;
    int unserialize(uint8 buffer[], int start);

    void dump();

    private:
    Ogre::Vector3 aim;

}; // class TurretControl
} // namespace ap

#endif
