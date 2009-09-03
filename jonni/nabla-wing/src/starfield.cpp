#include "starfield.h"

/**//* This mystical part here pushes exactly ONE object of this type to the prototype map, */
/**//* in order to easily unserialize stuff. Idea gotten from C++ FAQ Lite by Marshall Cline*/
/**/class StarFieldInject {
/**/    public: StarFieldInject() {
/**/        netobjectprototypes().insert(make_pair(ap::OBJECT_TYPE_STARFIELD, new StarField(0)));
/**/    }
/**/};
/**/static StarFieldInject __starfieldinject;
/**//* A create method like this is also ABSOLUTELY REQUIRED in descendants of NetObject. */
/**/NetObject *StarField::create(ap::uint32 id)
/**/{
/**/    NetObject *ptr = new StarField(id);
/**/    return ptr;
/**/}


StarField::StarField(int _id, int _uid)
{
    id = _id;   uid = _uid;
    changed = false;
}

uint8 StarField::getObjectType()
{
    return ap::OBJECT_TYPE_STARFIELD;
}

int StarField::serialize(ap::uint8 buffer[], int start, int buflength) const
{
    int length = 0;

    *(int *)(buffer+start+length) = id;                     length += 4;
    *(enet_uint8 *)(buffer+start+length) = ap::OBJECT_TYPE_STARFIELD; length++;

    return length;
}

int StarField::unserialize(ap::uint8 buffer[], int start)
{
    int length = 0;
    enet_uint8 objtype;

    if (id == *(uint32 *)(buffer+start)) {                     length += 4;
        objtype = *(enet_uint8 *)(buffer+start+length);     length++;
    } else cout << "FOULED in StarField::unserialize!" << endl;

    return length;
}
