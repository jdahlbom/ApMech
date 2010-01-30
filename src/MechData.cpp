#include "MechData.h"
#include "net/serializer.h"

namespace ap {

/**//* This mystical part here pushes exactly ONE object of this type to the prototype map, */
/**//* in order to easily unserialize stuff. Idea gotten from C++ FAQ Lite by Marshall Cline*/
/**/class MechDataInject {
/**/  public:
/**/    MechDataInject() {
/**/      ap::net::netobjectprototypes().insert(
/**/        std::make_pair(ap::OBJECT_TYPE_MECHDATA, new MechData()));
/**/    }
/**/};
/**/static MechDataInject __mechdatainject;


net::NetObject * MechData::create(uint32 id) const
{
    ap::net::NetObject *pNetObject = new MechData();
    pNetObject->id = id;
    return pNetObject;
}

int MechData::serialize(uint8 buffer[], int start, int buflength) const
{
    int length = 0;
    length += ap::net::serialize(name, buffer, start+length, buflength-length);
    length += ap::net::serialize(maxForwardAcceleration, buffer, start+length, buflength-length);
    length += ap::net::serialize(maxBackwardAcceleration, buffer, start+length, buflength-length);
    length += ap::net::serialize(maxTurnRate, buffer, start+length, buflength-length);
    length += ap::net::serialize(maxTorsoAngle, buffer, start+length, buflength-length);
    return length;
}

int MechData::unserialize(uint8 buffer[], int start)
{
    int length = 0;
    length += ap::net::unserialize(name, buffer, start+length);
    length += ap::net::unserialize(maxForwardAcceleration, buffer, start+length);
    length += ap::net::unserialize(maxBackwardAcceleration, buffer, start+length);
    length += ap::net::unserialize(maxTurnRate, buffer, start+length);
    length += ap::net::unserialize(maxTorsoAngle, buffer, start+length);
    return length;
}

ap::uint8 MechData::getObjectType() const {
    return ap::OBJECT_TYPE_MECHDATA;
}

}
