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

    // NetObject interface
    net::NetObject *create(uint32 id) const;
    int advance(float dt);

    uint8 getObjectType() const;
};

} // namespace ap

#endif
