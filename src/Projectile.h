#ifndef AP_PROJECTILE_H
#define AP_PROJECTILE_H

#include "MovingObject.h"
#include "types.h"
#include "constants.h"

#ifndef WIN32
#include <OgreVector3.h>
#else
#include <Ogre/OgreVector3.h>
#endif

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
