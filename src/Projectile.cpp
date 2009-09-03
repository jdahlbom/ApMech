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
    MovingObject(0.0f, velocity)
{
    objectType = ap::OBJECT_TYPE_PROJECTILE;
}

net::NetObject *Projectile::create(uint32 id)
{
    Projectile *proj = new Projectile(Ogre::Vector3::ZERO);
    proj->id = id;
    return proj;
}

uint8 Projectile::getObjectType()
{
    return ap::OBJECT_TYPE_PROJECTILE;
}

} // namespace ap
