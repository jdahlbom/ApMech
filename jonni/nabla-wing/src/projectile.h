#ifndef __projectile_h__
#define __projectile_h__

#include <cmath>     // for sin and cos, for advance
#include "netdata.h"
#include "netobject.h"
#include "location.h"

#include <SDL.h>

class Projectile : public NetObject {
 public:

    Location loc;
    float age;   // age of the object, in seconds.

    Projectile(int _id, int _uid = -1);

    int serialize(enet_uint8 buffer[], int start, int buflength);
    int unserialize(enet_uint8 buffer[], int start);
    NetObject *create(int id);

    int advance(float dt);
    int draw(SDL_Surface *s, float x, float y);
};

#endif
