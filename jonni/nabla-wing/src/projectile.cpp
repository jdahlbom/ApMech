#include "projectile.h"

/**//* This mystical part here pushes exactly ONE object of this type to the prototype map, */
/**//* in order to easily unserialize stuff. Idea gotten from C++ FAQ Lite by Marshall Cline*/
/**/class ProjectileInject {
/**/    public: ProjectileInject() {
/**/        netobjectprototypes().insert(make_pair(ap::OBJECT_TYPE_PROJECTILE, new Projectile(0)));
/**/    }
/**/};
/**/static ProjectileInject __projectileinject;
/**//* A create method like this is also ABSOLUTELY REQUIRED in descendants of NetObject. */
/**/NetObject *Projectile::create(ap::uint32 id) const
/**/{
/**/    NetObject *ptr = new Projectile(id);
/**/    return ptr;
/**/}


Projectile::Projectile(int _id, int _uid)
{
    id = _id;   uid = _uid; age = 0.0;
    loc.radius = 2;
    changed = false;
}

uint8 Projectile::getObjectType() const
{
    return ap::OBJECT_TYPE_PROJECTILE;
}

int Projectile::serialize(enet_uint8 buffer[], int start, int buflength) const
{
    int length = 0;

    *(int *)(buffer+start+length) = id;                     length += 4;
    *(enet_uint8 *)(buffer+start+length) = ap::OBJECT_TYPE_PROJECTILE; length++;
    *(int *)(buffer+start+length) = uid;                    length += 4;
    *(unsigned int *)(buffer+start+length) = age;           length += 4;
    length += loc.serialize(buffer, start+length, buflength);

    return length;
}

int Projectile::unserialize(enet_uint8 buffer[], int start)
{
    int length = 0;
    enet_uint8 objtype;

    if (id == *(ap::uint32 *)(buffer+start)) {                     length += 4;
        objtype = *(enet_uint8 *)(buffer+start+length);     length++;
        uid = *(int *)(buffer+start+length);                length += 4;
        age = *(unsigned int*)(buffer+start+length);        length += 4;
        length += loc.unserialize(buffer, start+length);
    } else cout << "FOULED in Projectile::unserialize!" << endl;

    return length;
}




int Projectile::advance(float dt)
{
    loc.advance(dt);
    age += dt;
    if (age > 4) {
//        loc.x = loc.y = -1000.0; loc.xvel = loc.yvel = loc.a = 0.0; // Hide the ammo, in case it's not destroyed soon enough? Maybe not needed
        return -1;      // request deletion
    }
    return 0;
}
