#include "Projectile.h"

#include <OgreVector3.h>

#include "types.h"
#include "constants.h"
#include "net/netdata.h"

namespace ap {
/**//* This mystical part here pushes exactly ONE object of this type to the prototype map, */
/**//* in order to easily unserialiUntitled3ze stuff. Idea gotten from C++ FAQ Lite by Marshall Cline*/
/**/class ProjectileInject {
/**/  public: ProjectileInject() {
/**/    ap::net::netobjectprototypes().insert(
/**/      std::make_pair(ap::OBJECT_TYPE_PROJECTILE, new Projectile(Ogre::Vector3::ZERO)));
/**/    }
/**/};
/**/static ProjectileInject __projectileinject;

Projectile::Projectile(Ogre::Vector3 velocity) :
    MovingObject(0.0f, velocity),
    age(0.0f)
{
    objectType = ap::OBJECT_TYPE_PROJECTILE;
}

net::NetObject *Projectile::create(uint32 id) const
{
    Projectile *proj = new Projectile(Ogre::Vector3::ZERO);
    proj->id = id;
    return proj;
}

int Projectile::advance(float dt)
{
    age += dt;
    if (age > 6) return -1;     // Live for 6 seconds

    MovingObject::advance(dt);
    if (hitWorldBoundaries()) return -1;     // If we hit the world's edge, then die
    return 0;
}

uint8 Projectile::getObjectType() const
{
    return ap::OBJECT_TYPE_PROJECTILE;
}

} // namespace ap
