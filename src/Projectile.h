#ifndef AP_PROJECTILE_H
#define AP_PROJECTILE_H

#include "MovingObject.hpp"
#include "types.h"
#include "constants.h"

#include <OgreVector3.h>

namespace ap {

class Projectile : public MovingObject
{
 public:
    Projectile(Ogre::Vector3 velocity);
    ~Projectile() {}

    net::NetObject *create(uint32 id);

    uint8 getObjectType();
};

} // namespace ap

#endif
