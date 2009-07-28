#ifndef AP_PROJECTILE_H
#define AP_PROJECTILE_H

#include "MovingObject.hpp"

#include <Ogre.h>

namespace ap {

class Projectile : public MovingObject
{
 public:
    Projectile(Ogre::Vector3 velocity);
    ~Projectile() {}

    net::NetObject *create(int id);
};

} // namespace ap

#endif
