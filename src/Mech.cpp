#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#include <Carbon/Carbon.h>
#endif
#include "Mech.h"

#include <Ogre.h>

#include "net/netdata.h"

namespace ap {
/**//* This mystical part here pushes exactly ONE object of this type to the prototype map, */
/**//* in order to easily unserialize stuff. Idea gotten from C++ FAQ Lite by Marshall Cline*/
/**/class MechInject {
/**/  public:
/**/    MechInject() {
/**/      ap::net::netobjectprototypes().insert(
/**/        std::make_pair(net::NetData::OBJECT_TYPE_MECH, new Mech(Ogre::Vector3::ZERO)));
/**/    }
/**/};
/**/static MechInject __projectileinject;

Mech::Mech(Ogre::Vector3 velocity) :
    MovingObject(0.0f, velocity) {}

net::NetObject *Mech::create(int id)
{
    Mech *mech = new Mech(Ogre::Vector3::ZERO);
    mech->id = id;
    return mech;
}

} // namespace ap
