#ifndef AP_MECH_H
#define AP_MECH_H

#include "MovingObject.hpp"
#include "types.h"
#include "constants.h"

#include <OgreVector3.h>

namespace ap {

class Mech : public MovingObject
{
 public:
    uint32 color;

    Mech(Ogre::Vector3 velocity = Ogre::Vector3::ZERO);
    ~Mech() {}

    // NetObject interface .. that was not implemented by MovingObject
    net::NetObject *create(uint32 id) const;
    uint8 getObjectType() const;

    //NetObject serialization
    int serialize(uint8 *buffer, int start, int buflength) const;
    int unserialize(uint8 *buffer, int start);
};

} // namespace ap

#endif
