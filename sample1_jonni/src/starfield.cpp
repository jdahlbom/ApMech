#include "starfield.h"

/**//* This mystical part here pushes exactly ONE object of this type to the prototype map, */
/**//* in order to easily unserialize stuff. Idea gotten from C++ FAQ Lite by Marshall Cline*/
/**/class StarFieldInject {
/**/    public: StarFieldInject() {
/**/        netobjectprototypes().insert(make_pair(NetData::OBJECT_TYPE_STARFIELD, new StarField(0)));
/**/    }
/**/};
/**/static StarFieldInject __starfieldinject;
/**//* A create method like this is also ABSOLUTELY REQUIRED in descendants of NetObject. */
/**/NetObject *StarField::create(int id)
/**/{
/**/    NetObject *ptr = new StarField(id);
/**/    return ptr;
/**/}


StarField::StarField(int _id, int _uid)
{
    id = _id;   uid = _uid;
    changed = false;
}

int StarField::serialize(enet_uint8 buffer[], int start, int buflength)
{
    int length = 0;

    *(int *)(buffer+start+length) = id;                     length += 4;
    *(enet_uint8 *)(buffer+start+length) = NetData::OBJECT_TYPE_STARFIELD; length++;

    return length;
}

int StarField::unserialize(enet_uint8 buffer[], int start)
{
    int length = 0;
    enet_uint8 objtype;

    if (id == *(int *)(buffer+start)) {                     length += 4;
        objtype = *(enet_uint8 *)(buffer+start+length);     length++;
    } else cout << "FOULED!" << endl;

    return length;
}

int StarField::draw(SDL_Surface *s, float x, float y)
{
    const int stars = 10000;
    int *scr = (int *)(s->pixels);

    int seed = 452525936;
    int starx, stary, brightness;

    for (int i=0; i<stars; i++)
    {
        starx = (seed >> 16) % 4000;
        seed = ((214013*seed) + 2531011) & 0x7FFFFFFF;      // Step random func
        stary = (seed >> 16) % 4000;
        seed = ((214013*seed) + 2531011) & 0x7FFFFFFF;      // Step random func
        brightness = (seed >> 16) % 256;
        seed = ((214013*seed) + 2531011) & 0x7FFFFFFF;      // Step random func

        if ((starx >= 0) && (stary >= 0) && (starx < s->w) && (stary < s->h))
            scr[starx + stary*s->w] = brightness + brightness*256 + brightness*65536;

    }

    return 1;
}
