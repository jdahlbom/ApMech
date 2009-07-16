#include "Projectile.h"

#include <Ogre.h>

#include "net/netdata.h"

namespace ap {
/**//* This mystical part here pushes exactly ONE object of this type to the prototype map, */
/**//* in order to easily unserialiUntitled3ze stuff. Idea gotten from C++ FAQ Lite by Marshall Cline*/
/**/class ProjectileInject {
/**/  public: ProjectileInject() {
/**/    ap::net::netobjectprototypes().insert(
/**/      std::make_pair(net::NetData::OBJECT_TYPE_PROJECTILE, new Projectile(Ogre::Vector3::ZERO)));
/**/    }
/**/};
/**/static ProjectileInject __projectileinject;

Projectile::Projectile(Ogre::Vector3 velocity) :
    MovingObject(0.0f, velocity) {}

net::NetObject *Projectile::create(int id)
{
    Projectile *proj = new Projectile(Ogre::Vector3::ZERO);
    proj->id = id;
    return proj;
}

} // namespace ap
