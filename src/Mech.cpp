#include "Mech.h"

#include <OgreVector3.h>

#include "types.h"
#include "constants.h"
#include "net/netdata.h"

namespace ap {
/**//* This mystical part here pushes exactly ONE object of this type to the prototype map, */
/**//* in order to easily unserialize stuff. Idea gotten from C++ FAQ Lite by Marshall Cline*/
/**/class MechInject {
/**/  public:
/**/    MechInject() {
/**/      ap::net::netobjectprototypes().insert(
/**/        std::make_pair(ap::OBJECT_TYPE_MECH, new Mech(Ogre::Vector3::ZERO)));
/**/    }
/**/};
/**/static MechInject __projectileinject;

Mech::Mech(Ogre::Vector3 velocity) :
    MovingObject(0.0f, velocity)
{
    objectType = ap::OBJECT_TYPE_MECH;
}

net::NetObject *Mech::create(uint32 _id)
{
    Mech *mech = new Mech(Ogre::Vector3::ZERO);
    mech->id = _id;
    return mech;
}

uint8 Mech::getObjectType()
{
    return ap::OBJECT_TYPE_MECH;
}

} // namespace ap
