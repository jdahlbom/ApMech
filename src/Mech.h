#ifndef AP_MECH_H
#define AP_MECH_H

#include "MovingObject.hpp"

#include <OgreVector3.h>

namespace ap {

class Mech : public MovingObject
{
 public:
    Mech(Ogre::Vector3 velocity = Ogre::Vector3::ZERO);
    ~Mech() {}

    net::NetObject *create(uint32 id);
};

} // namespace ap

#endif
