#ifndef AP_PROJECTILE_H
#define AP_PROJECTILE_H

#include "MovingObject.hpp"

#include <OgreVector3.h>

namespace ap {

class Projectile : public MovingObject
{
 public:
    Projectile(Ogre::Vector3 velocity);
    ~Projectile() {}

    net::NetObject *create(uint32 id);
};

} // namespace ap

#endif
