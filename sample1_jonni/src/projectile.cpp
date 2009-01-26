#include "projectile.h"

/**//* This mystical part here pushes exactly ONE object of this type to the prototype map, */
/**//* in order to easily unserialize stuff. Idea gotten from C++ FAQ Lite by Marshall Cline*/
/**/class ProjectileInject {
/**/    public: ProjectileInject() {
/**/        netobjectprototypes().insert(make_pair(NetData::OBJECT_TYPE_PROJECTILE, new Projectile(0)));
/**/    }
/**/};
/**/static ProjectileInject __projectileinject;
/**//* A create method like this is also ABSOLUTELY REQUIRED in descendants of NetObject. */
/**/NetObject *Projectile::create(int id)
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

int Projectile::serialize(enet_uint8 buffer[], int start, int buflength)
{
    int length = 0;

    *(int *)(buffer+start+length) = id;                     length += 4;
    *(enet_uint8 *)(buffer+start+length) = NetData::OBJECT_TYPE_PROJECTILE; length++;
    *(int *)(buffer+start+length) = uid;                    length += 4;
    *(unsigned int *)(buffer+start+length) = age;           length += 4;
    length += loc.serialize(buffer, start+length, buflength);

    return length;
}

int Projectile::unserialize(enet_uint8 buffer[], int start)
{
    int length = 0;
    enet_uint8 objtype;

    if (id == *(int *)(buffer+start)) {                     length += 4;
        objtype = *(enet_uint8 *)(buffer+start+length);     length++;
        uid = *(int *)(buffer+start+length);                length += 4;
        age = *(unsigned int*)(buffer+start+length);        length += 4;
        length += loc.unserialize(buffer, start+length);
    } else cout << "FOULED!" << endl;

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

int Projectile::draw(SDL_Surface *s, float x, float y)
{
    int sx, sy;
    sx = fmod(loc.x + 6000.0 -x + s->w/2.0, 4000.0) - 2000.0;
    sy = fmod(-loc.y + 6000.0 +y + s->h/2.0, 4000.0) - 2000.0;

    SDL_Rect dstrect;
    dstrect.w = dstrect.h = 3;
    dstrect.x = sx; //(s->w)/2 + loc.x - 1 - x;
    dstrect.y = sy; //(s->h)/2 - loc.y + 1 + y;
    SDL_FillRect(s, &dstrect, 0x00FFAA00);
    return 1;
}
