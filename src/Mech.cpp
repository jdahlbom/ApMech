#include "Mech.h"

#ifndef WIN32
#include <OgreVector3.h>
#else
#include <Ogre/OgreVector3.h>
#endif

#include "net/serializer.h"
#include "ObjectDataModel.h"

namespace ap {
/**//* This mystical part here pushes exactly ONE object of this type to the prototype map, */
/**//* in order to easily unserialize stuff. Idea gotten from C++ FAQ Lite by Marshall Cline*/
/**/class MechInject {
/**/  public:
/**/    MechInject() {
/**/      ap::net::netobjectprototypes().insert(
/**/        std::make_pair(ap::OBJECT_TYPE_MECH, new Mech(Ogre::Vector3::ZERO, NULL)));
/**/    }
/**/};
/**/static MechInject __projectileinject;

Mech::Mech(Ogre::Vector3 velocity, ObjectDataModel *model) :
    MovingObject(0.0f, velocity, model, ap::OBJECT_TYPE_MECH)
{
    objectType = ap::OBJECT_TYPE_MECH;
}

net::NetObject *Mech::create(uint32 _id) const
{
    Mech *mech = new Mech(Ogre::Vector3::ZERO, ObjectDataModel::getInstance());
    mech->id = _id;
    return mech;
}

uint8 Mech::getObjectType() const
{
    return ap::OBJECT_TYPE_MECH;
}


int Mech::serialize(uint8 *buffer, int start, int buflength) const
{
    using ap::net::serialize;

    int length = 0;
    length += MovingObject::serialize(buffer, start+length, buflength);
    length += serialize(color, buffer, start+length, buflength);
    return length;
}

int Mech::unserialize(uint8 *buffer, int start)
{
    using ap::net::unserialize;

    int length = 0;
    length += MovingObject::unserialize(buffer, start+length);
    length += unserialize(color, buffer, start+length);
    return length;
}

} // namespace ap
