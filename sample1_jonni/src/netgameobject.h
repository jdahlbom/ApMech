#ifndef __netgameobject_h__
#define __netgameobject_h__

#include <list>
#include <cmath>     // for sin and cos, for advance
#include "netdata.h"
#include "netobject.h"
#include "netuser.h"
#include "location.h"
#include "projectile.h"
#include <iostream>

#include <SDL.h>        // for draw()

class NetGameObject : public NetObject {

 public:

    int id, color;      // transported over network
    float mg_delay;

    Location loc;

    NetGameObject(int _id, int _uid = -1);

    int serialize(enet_uint8 buffer[], int start, int buflength);
    int unserialize(enet_uint8 buffer[], int start);
    NetObject *create(int id);

    int advance(float dt);
    int draw(SDL_Surface *s, float x, float y);
    list<NetObject *> *control(NetUser &user, bool createobjects = false);
};

#endif
