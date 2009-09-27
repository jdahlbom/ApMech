#ifndef AP_PROJECTILE_H
#define AP_PROJECTILE_H

#include "MovingObject.hpp"
#include "types.h"
#include "constants.h"

#include <OgreVector3.h>

namespace ap {

class Projectile : public MovingObject
{
 private:
    float age;

 public:
    Projectile(Ogre::Vector3 velocity);
    ~Projectile() {}

    net::NetObject *create(uint32 id);
    int advance(float dt);

    uint8 getObjectType();
};

} // namespace ap

#endif
